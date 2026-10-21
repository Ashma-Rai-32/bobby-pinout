const char PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Bobby Remote</title>
  <style>
    body   { font-family: sans-serif; text-align: center; margin-top: 40px; }
    button { padding: 12px 24px; font-size: 18px; }
  </style>
</head>
<body>
  <h1>Bobby Remote</h1>
  <button onclick="send('./toggle')">Toggle LED</button>

  <p id="reply"></p>
 <input type="text" id="msg" placeholder="Enter text to show on OLED">
 <button onclick="sendMsg()">Send</button>

 <button onclick="send('./invert')">Invert OLED</button>
 <button onclick="send('./clear')">Clear OLED</button>
  <script>
    function toggleLED() {
      fetch('/toggle');
    }

  function send(path) {
    fetch(path)
      .then(r => r.text())
      .then(t => document.getElementById('reply').textContent = t);
  }

  function sendMsg(){
  const text= document.getElementById('msg').value;
  send('/msg?text=' + encodeURIComponent(text));
  }

  </script>
</body>
</html>
)rawliteral";
