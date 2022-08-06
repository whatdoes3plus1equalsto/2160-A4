main: ObjectManager.o main0.c
	clang++ -Wall main.c ObjectManager.o -o main
ObjectManager.o: ObjectManager.c ObjectManager.h
	clang++ -Wall ObjectManager.c -o ObjectManager.o
clean:
	rm -fr *.o main