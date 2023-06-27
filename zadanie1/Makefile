CC = g++
FLAGS = -Wall -W -Wextra -std=c++11 -pedantic -pedantic-errors -Werror -Wreturn-type

traceroute: myreceive.o mysend.o utils.o traceroute.cpp
	$(CC) $(FLAGS) -o traceroute myreceive.o mysend.o utils.o traceroute.cpp

myreceive: myreceive.cpp myreceive.hpp utils.o utils.hpp
	$(CC) $(FLAGS) -c -o myreceive.o myreceive.cpp utils.o 

mysend: mysend.cpp mysend.hpp utils.o utils.hpp
	$(CC) $(FLAGS) -c -o mysend.o mysend.cpp utils.o

utils: utils.hpp utils.cpp
	$(CC) $(FLAGS) -c -o utils.o utils.cpp

clean:
	rm -f *.o
	
distclean:
	rm -f traceroute

