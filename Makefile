all:
	g++ -std=c++17 main.cpp ./src/*.cpp -o Tetris.exe -mwindows -Isrc/include -Lsrc/lib -lSDL3 -lSDL3_image -lSDL3_ttf
	./Tetris.exe
