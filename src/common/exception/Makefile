OBJECTS	= $(patsubst %.cpp, %.o, $(wildcard *.cpp))

all : objs
objs : $(OBJECTS)
	cp -f $^ $(PIPEY_ROOT)

clean :
	rm -rf *.o 
