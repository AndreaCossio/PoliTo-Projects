COMPILER = gcc
OPTIONS = -Wall
SOURCES = $(shell find . -name "*.c")
OUTPUT = exe

all:
	$(COMPILER) $(OPTIONS) -o $(OUTPUT) $(SOURCES) -pthread

install:
	$(COMPILER) $(OPTIONS) -o $(OUTPUT) $(SOURCES) -pthread
	mkdir ../bin
	mv $(OUTPUT) ../bin

clean:
	rm -rf $(OUTPUT)

distclean:
	make clean
	rm -rf ../bin
