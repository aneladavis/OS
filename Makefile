OBJECTS1 = frame.o test_1.o ll_double.o 
OBJECTS2 = translation.o test_2.o ll_double.o

pc1-clean: clean pc1
pc2-clean: clean pc2

pc1: $(OBJECTS1)
	clang -g -o pc1 $(OBJECTS1) -I.
pc2: $(OBJECTS1)
	clang -g -o pc2 $(OBJECTS1) -I.

%.o: %.c
	clang -g -c -o $@ -I. -Inet $<

clean:
	rm -f *.o pc1 pc2