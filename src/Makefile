#executable name
EXEC=NINMAP

#Source files
SRCS=algorithm.cpp compiler.cpp flag.cpp expression.cpp main.cpp token.cpp

#Object files
OBJS=$(SRCS:.cpp=.o)

$(EXEC):$(OBJS)
	g++ -std=c++11 -o $@ $(OBJS)

$(OBJS):
	g++ -std=c++11 -c $*.cpp

clean:
	rm $(OBJS)
