HOBJS = integer.o real.o string.o dll.o sll.o stack.o queue.o bst.o scanner.o heap.o heapsort.o
BSTOBJS = integer.o real.o string.o sll.o queue.o bst.o test-bst.o
OOPTS = -Wall -Wextra -std=c99 -c -g
EOPTS = -Wall -Wextra -std=c99 -g
VOPTS = -v
IOPTS = -i -I data
IDOPTS = -i -D data
ROPTS = -r -I data1
RDOPTS = -r -D data1
SOPTS = -s -I data2
SDOPTS = -s -D data2

all : test-bst heapsort

heapsort : $(HOBJS)
	gcc $(EOPTS) -o heapsort $(HOBJS)

test-bst : $(BSTOBJS)
	gcc $(EOPTS) -o test-bst $(BSTOBJS)

dll.o : dll.c dll.h
	gcc $(OOPTS) dll.c

sll.o : sll.c sll.h
	gcc $(OOPTS) sll.c

stack.o : stack.c stack.h dll.h
	gcc $(OOPTS) stack.c

queue.o : queue.c queue.h sll.h
	gcc $(OOPTS) queue.c

integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c

real.o : real.c real.h
	gcc $(OOPTS) real.c

string.o : string.c string.h
	gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c

bst.o : bst.c bst.h
	gcc $(OOPTS) bst.c

heapsort.o : heapsort.c integer.h real.h string.h dll.h sll.h stack.h queue.h bst.h scanner.h heap.h 
	gcc $(OOPTS) heapsort.c

heap.o : heap.c integer.h real.h string.h dll.h sll.h stack.h queue.h bst.h heap.h
	gcc $(OOPTS) heap.c

test-bst.o : test-bst.c integer.h real.h string.h sll.h queue.h
	gcc $(OOPTS) test-bst.c

valgrind : test-bst
	@echo testing bst
	valgrind ./test-bst

test : all
	@echo running: heapsort -v
	./heapsort $(VOPTS)
	@echo
	@echo running: heapsort -i -I data
	./heapsort $(IOPTS)
	@echo
	@echo running: heapsort -i -D data
	./heapsort $(IDOPTS)
	@echo
	@echo running: heapsort -r -I data1
	./heapsort $(ROPTS)
	@echo
	@echo running: heapsort -r -D data1
	./heapsort $(RDOPTS)
	@echo
	@echo running: heapsort -s -I data2
	./heapsort $(SOPTS)
	@echo
	@echo running: heapsort -s -D data2
	./heapsort $(SDOPTS)

clean   :
	rm -f *.o heapsort test-bst


