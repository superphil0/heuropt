ifdef SystemRoot
   RM = del /Q
   FixPath = $(subst /,\,$1)
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
      FixPath = $1
   endif
endif


all: hot

hot: main.o page.gch edge.gch node.gch deterministic.o edgeswap.o nodeswap.o kpmp_instance.o kpmp_solution_writer.o
	g++ -std=c++11 main.o deterministic.o edgeswap.o nodeswap.o kpmp_instance.o kpmp_solution_writer.o -o hot

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

edge.gch: edge.h node.h
	g++ -std=c++11 -c edge.h

page.gch: page.h edge.h
	g++ -std=c++11 -c page.h

node.gch: node.h
		g++ -std=c++11 -c node.h

deterministic.o: deterministic.cpp deterministic.h
	g++ -std=c++11 -c deterministic.cpp

edgeswap.o: edgeswap.cpp edgeswap.h
	g++ -std=c++11 -c edgeswap.cpp

nodeswap.o: nodeswap.cpp nodeswap.h
	g++ -std=c++11 -c nodeswap.cpp

kpmp_instance.o: kpmp_instance.cpp kpmp_instance.h
	g++ -std=c++11 -c kpmp_instance.cpp

kpmp_solution_writer.o: kpmp_solution_writer.cpp kpmp_solution_writer.h
	g++ -std=c++11 -c kpmp_solution_writer.cpp

clean:
	$(RM) *.o *.gch *.exe
