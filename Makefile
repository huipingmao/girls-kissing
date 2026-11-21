build/game: main.cpp
	g++ -std=c++17 main.cpp -o build/game -lraylib -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-missing-field-initializers -Wno-parentheses -Wno-unused-but-set-variable
run:
	./build/game
.PHONY: run clang runeditor
clang:
	clang main.cpp -o build/game-clang -lraylib -std=c++17
e:
	cc -o build/gk-editor level-editor.c -lraylib -std=c99 -Wall -Wextra -lm -Wno-unused-parameter -Wno-unused-function
rune:
	./build/gk-editor
