source = *.cpp
output = Tetris_v2.exe

all:
# clear screen
	cls
# compile
	g++ $(source) -o $(output) -std=c++20 -Isrc/include -Lsrc/lib \
	-lSDL3 -lSDL3_image -DLOCAL -ggdb3 -Wall -Wextra -Wunused-variable \
	-pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wlogical-op \
	-Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align \
	-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 \
	-D_GLIBCXX_ASSERTIONS -fno-sanitize-recover -fstack-protector -xc++
# run
	./$(output)