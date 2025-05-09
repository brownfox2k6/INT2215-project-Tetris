all:
	g++ main.cpp ./src/*.cpp -oTetris.exe -Isrc/include -Lsrc/lib -lSDL3 -lSDL3_image -lSDL3_ttf -mwindows
	./Tetris.exe
