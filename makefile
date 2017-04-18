CXX=g++
CFLAGS=-Wall -std=c++14


DEPS=token.h lexer.h 
OBJ=main.o token.o lexer.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) 

icingaParser: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm *.o
