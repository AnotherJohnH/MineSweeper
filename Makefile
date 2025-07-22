
targets = native
# Emscripten microbitV2

.PHONY: all
all: $(targets)

.PHONY: docs
docs: Emscripten
	cp build_Emscripten/mines.html docs
	cp build_Emscripten/mines.js   docs
	cp build_Emscripten/mines.wasm docs
	doxygen

include Platform/build.make
