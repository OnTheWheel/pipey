DIRS	= common memory thread util 
all : objs

objs :
	@for dir in $(DIRS); do \
	    make -C $$dir || exit $?; \
	    done

clean :
	@for dir in $(DIRS); do \
	    make -C $$dir clean; \
	    done
