TARGET=fs21
TARGET_TEST=fst21
CC=g++
FLAGS= -std=c++11
PREFIX=/usr/local/bin



all:$(TARGET)
$(TARGET):funny_snake21.o game.o unit.o
	$(CC)  -g -o $(TARGET) funny_snake21.o game.o unit.o `pkg-config --libs gtkmm-3.0`
funny_snake21.o:funny_snake21.cpp
	$(CC)  `pkg-config --cflags gtkmm-3.0` -g -c -o  funny_snake21.o funny_snake21.cpp 
game.o:game.cpp game.h
	$(CC) -g -c -o game.o game.cpp
unit.o:unit.cpp unit.h
	$(CC) -g -c -o unit.o unit.cpp

test:$(TARGET_TEST)
$(TARGET_TEST):funny_snake21_test.o game.o unit.o
	$(CC) -g -o $(TARGET_TEST) funny_snake21_test.o game.o unit.o -lncurses
funny_snake21_test.o:funny_snake21_test.cpp
	$(CC) -g -c -o funny_snake2_test.o funny_snake2_test.cpp


clean:
	rm -rf  $(TARGET) *.o $(TARGET_TEST)

install:
	install $(TARGET) $(PREFIX)

uninstall:
	rm -rf $(PREFIX)/$(TARGET)
