all: build/game build/editor

build/game: main.cpp
	mkdir -p build
	g++ -std=c++17 main.cpp -o build/game -lraylib -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-missing-field-initializers -Wno-parentheses -Wno-unused-but-set-variable
build/editor: level-editor.c
	mkdir -p build
	cc -o build/editor level-editor.c -lraylib -std=c99 -Wall -Wextra -lm -Wno-unused-parameter -Wno-unused-function

e: build/editor
	./build/editor level-1.lesbian

g: build/game
	./build/game

# clang: main.cpp
# 	mkdir -p build
# 	clang main.cpp -o build/game-clang -lraylib -std=c++17

.PHONY: g e
