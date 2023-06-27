CC = g++
FLAGS = -Wall -W -Wextra -std=c++11 -pedantic -pedantic-errors -Werror -Wreturn-type

router: receiving.o sending.o utils.o router.cpp
	$(CC) $(FLAGS) -o router receiving.o sending.o utils.o router.cpp

receiving: receiving.cpp receiving.hpp utils.o utils.hpp
	$(CC) $(FLAGS) -c -o receiving.o receiving.cpp utils.o

sending: sending.cpp sending.hpp utils.o utils.hpp
	$(CC) $(FLAGS) -c -o sending.o sending.cpp utils.o

utils: utils.hpp utils.cpp
	$(CC) $(FLAGS) -c -o utils.o utils.cpp

clean:
	rm -f *.o
	
distclean:
	rm -f router