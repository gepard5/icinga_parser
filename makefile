CXX=g++
CFLAGS=-Wall -lsfml-window -lsfml-system -lsfml-graphics -std=c++14


DEPS=token.h lexer.h fileSource.h source.h stringSource.h parser_exceptions.h parser.h icinga_object.h icinga_parser.h drawingobject.h
OBJ=main.o token.o lexer.o fileSource.o stringSource.o parser.o icinga_object.o icinga_parser.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) 

icingaParser: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm *.o
