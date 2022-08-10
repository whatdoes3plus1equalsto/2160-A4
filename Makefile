main: ObjectManager.o main.o
	clang++ -Wall main.o ObjectManager.o -o main
ObjectManager.o: ObjectManager.c ObjectManager.h
	clang++ -Wall -c ObjectManager.c -o ObjectManager.o
main.o: main2.c
	clang++ -Wall -c main2.c -o main.o
clean:
	rm -fr *.o main