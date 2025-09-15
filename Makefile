.PHONY: all debug release clean run rebuild

all: debug

debug:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --config Debug

release:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build --config Release

clean:
	rm -rf build

run: debug
	./build/ARC

run-release: release
	./build/ARC

rebuild: clean debug
