all: lib/sqlite_wrapper.cpp
	g++ -std=c++17 -c lib/sqlite_wrapper.cpp -o sqlitewrap.o
	g++ -std=c++17 main2.cpp sqlitewrap.o -lsqlite3
	ar rcs libsqlitewrap.a sqlitewrap.o
	mv *.o build/
	mv *.a build/

clean:
	rm build/*.o
	rm a.out