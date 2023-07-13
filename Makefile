#configurer project
all: configurate build run
configurate_test:
	cmake -DGLFW_BUILD_DOCS=OFF -S . -B out/build -G "MinGW Makefiles" CMakeLists.txt
configurate:
	cmake -DGLFW_BUILD_DOCS=OFF -S . -B out/build CMakeLists.txt
#build
build_test:
	cd out/build && mingw32-make 
build:
	cd out/build && cmake --build . --target GraphicApp --config Debug
#run
run_test:
	./out/build/GraphicApp
run:
	./out/build/Debug/GraphicApp

build_run: build run

delete:
	rm -rf out/build
	cd out && mkdir build
name:
	echo $(uname)