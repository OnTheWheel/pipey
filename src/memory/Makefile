DIRS = pool buffer
OBJECTS	= $(filter-out Windows%, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))

all : sub_dirs

objs : $(OBJECTS)
	cp -f $^ $(PIPEY_ROOT)

sub_dirs : $(DIRS)
	 @for dir in $(DIRS); do \
	 	make -C $$dir || exit $?; \
	 	done


clean :
	rm -rf *.o 
