DIRS = sync
OBJECTS	= $(filter-out Windows%, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))

all : objs sub_dirs

objs : $(OBJECTS)
	cp -f $^ $(PIPEY_ROOT)

sub_dirs : $(DIRS)
	@for dir in $(DIRS); do \
	    make -C $$dir || exit $?; \
	    done

clean :
	@for dir in $(DIRS); do \
	    make -C $$dir clean; \
	    done
	rm -rf *.o 
