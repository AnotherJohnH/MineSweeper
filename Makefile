
EXECUTABLE = mines

#-------------------------------------------------------------------------------

all: native Emscripten

clean:
	rm -rf build_*

debug:
	Platform/BUILD.py native -s -d

#-------------------------------------------------------------------------------

docs: Emscripten
	cp build_Emscripten/mines.html docs/Emscripten
	cp build_Emscripten/mines.js   docs/Emscripten
	cp build_Emscripten/mines.wasm docs/Emscripten
	doxygen

#-------------------------------------------------------------------------------

native:
	Platform/BUILD.py native

Emscripten:
	Platform/BUILD.py Emscripten

mbedLPC1768:
	Platform/BUILD.py mbedLPC1768
