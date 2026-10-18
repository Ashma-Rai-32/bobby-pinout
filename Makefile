# PORT comes from .env (the real board's USB port)
-include .env

SIM_FQBN   = esp32:esp32:esp32c3
BOARD_FQBN = esp32:esp32:esp32c3:CDCOnBoot=cdc

# No L = main project. L=02-serial = that lesson.
SKETCH = $(if $(L),lessons/$(L)/$(L).ino,bobby-pinout.ino)
DIR    = $(dir $(SKETCH))

.PHONY: build dev upload monitor

# Compile for Wokwi once, and say clearly whether it worked
build:
	@arduino-cli compile --fqbn $(SIM_FQBN) --output-dir $(DIR)build $(DIR) \
		&& echo "\n✅ BUILD OK $(SKETCH) $$(date +%H:%M:%S)\n" \
		|| (echo "\n❌ BUILD FAILED $(SKETCH) $$(date +%H:%M:%S) (simulator is still running the old code)\n"; exit 1)

# Rebuild for Wokwi every time the sketch is saved (needs: brew install fswatch)
dev:
	-@$(MAKE) --no-print-directory build
	@fswatch -o --latency 0.2 $(SKETCH) | xargs -n1 -I{} $(MAKE) --no-print-directory build

# Compile for the real SuperMini and upload it
upload:
	arduino-cli compile -u -p $(PORT) --fqbn $(BOARD_FQBN) $(DIR)

# Show Serial output from the real board (Ctrl+C to exit)
monitor:
	arduino-cli monitor -p $(PORT) -c baudrate=115200
