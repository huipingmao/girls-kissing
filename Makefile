build/game: main.cpp
	g++ -std=c++17 main.cpp -o build/game -lraylib -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-missing-field-initializers -Wno-parentheses -Wno-unused-but-set-variable
run:
	./build/game
.PHONY: run
