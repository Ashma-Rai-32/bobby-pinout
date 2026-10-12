FQBN = esp32:esp32:esp32c3

.PHONY: build dev

# Compile once
build:
	arduino-cli compile --fqbn $(FQBN) --output-dir build .

# Rebuild every time the sketch is saved (needs: brew install fswatch)
dev: build
	fswatch -o bobby-pinout.ino | xargs -n1 -I{} make build
