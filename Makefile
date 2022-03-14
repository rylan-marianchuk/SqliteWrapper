all:
	g++ -std=c++17 -c sqlite_wrapper.cpp -o sqlitewrap.o
	g++ -std=c++17 main2.cpp sqlitewrap.o -lsqlite3

clean:
	rm *.o
	rm a.out
	rm *.db