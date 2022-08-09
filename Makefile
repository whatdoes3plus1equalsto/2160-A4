main: ObjectManager.o main0.o
	clang++ -Wall main0.o ObjectManager.o -o main
ObjectManager.o: ObjectManager.c ObjectManager.h
	clang++ -Wall -c ObjectManager.c -o ObjectManager.o
main0.o: main0.c
	clang++ -Wall -c main0.c -o main0.o
clean:
	rm -fr *.o main