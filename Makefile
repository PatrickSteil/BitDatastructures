CC=g++
FLAGS=-std=c++17 -pipe
OPTIMIZATION=-march=native -O3
DEBUG=-rdynamic -Werror -Wpedantic -pedantic-errors -Wall -Wextra -Wparentheses -Wfatal-errors -D_GLIBCXX_DEBUG -g -fno-omit-frame-pointer
RELEASE=-ffast-math -ftree-vectorize -Wfatal-errors -DNDEBUG

Debug:
	$(CC) $(FLAGS) $(OPTIMIZATION) $(DEBUG) -o ads_programm ads_programm.cpp
Test:
	$(CC) $(FLAGS) $(OPTIMIZATION) $(DEBUG) -o BVTest BitVector/BitVectorTest.cpp 
	$(CC) $(FLAGS) $(OPTIMIZATION) $(DEBUG) -o EFTest EliasFano/EFTest.cpp 

Release:
	$(CC) $(FLAGS) $(OPTIMIZATION) $(RELEASE) -o ads_programm ads_programm.cpp
