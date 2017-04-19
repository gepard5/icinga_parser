CXX=g++
CFLAGS=-Wall -std=c++14


DEPS=token.h lexer.h fileSource.h source.h stringSource.h
OBJ=main.o token.o lexer.o fileSource.o stringSource.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) 

icingaParser: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm *.o
