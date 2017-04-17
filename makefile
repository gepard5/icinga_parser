CXX=g++
CFLAGS=-Wall -std=c++14
LIBS=-lboost_program_options

DEPS=token.h lexer.h 
OBJ=main.o token.o lexer.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)

icingaParser: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o
