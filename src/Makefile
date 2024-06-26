OC := $(shell uname)
CC = g++ -std=c++17
CFLAGS = -Wall -Werror -Wextra

ifeq ($(OC),Darwin)
	TFLAGS = -lgtest -lstdc++
else
	TFLAGS = -lgtest -lstdc++ -pthread
endif

all: clean install

buildapp:
	mkdir -p build
	cd ./build; qmake ../; make
	
install: buildapp
ifeq ($(OC),Darwin)
	cp -rf build/maze.app ~/Applications/Maze.app
else
	mkdir -p app
	install build/maze app/Maze
endif
	
uninstall:
	rm -rf build
ifeq ($(OC),Darwin)
	rm -rf ~/Applications/Maze.app
else
	rm -rf app
endif

dist: install
ifeq ($(OC),Darwin)
	tar --totals --create --verbose --file maze.tar ~/Applications/Maze.app
else
	tar -zcvf maze.tar app/*
endif

tests:
	cd test; $(CC) test_main.cpp ../model/maze.cpp ../model/cave.cpp -o test_out $(TFLAGS) $(CFLAGS)
	./test/test_out

dvi:
	$(TEXI2DVI) documentation.tex

pdf:
	$(TEXI2DVI) -p documentation.tex
	
linter:
	cp ../materials/linters/.clang-format .
	clang-format -n test/*.cpp controller/*.cpp controller/*.h model/*.cpp model/*.h view/*.cpp view/*.h main.cpp --verbose
		
clean:
	rm -rf build/
	rm -rf *.aux *.log *.toc *.pdf *.dvi *.tar .clang-format
	rm -rf test/*_out test/*.o test/testW*.txt
