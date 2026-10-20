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
  <button onclick="toggleLED()">Toggle LED</button>

  <script>
    function toggleLED() {
      fetch('/toggle');
    }
  </script>
</body>
</html>
)rawliteral";
