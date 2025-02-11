CC=g++
CFLAGS=-c -Wall -std=c++11 -O3
LDFLAGS=
SOURCES=idastar.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=blocs

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)

re: clean all
