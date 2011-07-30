export PIPEY_ROOT = $(shell pwd)
export CC	= g++
DIRS	= src
OBJECTS	= *.o
TARGET	= pipey

all : objs
	$(CC) -c -o $(TARGET).o $(TARGET).cpp 
	$(CC) -o $(TARGET) $(OBJECTS) -lpthread -lc

objs :
	@for dir in $(DIRS); do \
	    make -C $$dir || exit $?; \
	    done

clean :
	@for dir in $(DIRS); do \
	    make -C $$dir clean; \
	    done
	    -rm -rf *.o $(TARGET)
