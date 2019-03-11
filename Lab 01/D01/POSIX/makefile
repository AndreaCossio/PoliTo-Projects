COMPILER = gcc
OPTIONS = -Wall
SOURCES = $(shell find . -name "*.c")
OUTPUT = exe

all:
	$(COMPILER) $(OPTIONS) -o $(OUTPUT) $(SOURCES)

install:
	$(COMPILER) $(OPTIONS) -o $(OUTPUT) $(SOURCES)
	mkdir ../bin
	mv $(OUTPUT) ../bin

clean:
	rm -rf $(OUTPUT)
	rm -rf fv*

distclean:
	make clean
	rm -rf ../bin
