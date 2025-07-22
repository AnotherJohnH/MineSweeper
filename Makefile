
targets = native
# Emscripten microbitV2

.PHONY: all
all: $(targets)

.PHONY: docs
docs: Emscripten
	cp build/Emscripten/mines.html docs
	cp build/Emscripten/mines.js   docs
	cp build/Emscripten/mines.wasm docs

include Platform/build.make
