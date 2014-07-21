CC=g++
CFLAGS=-O3 -funroll-loops -c -Wno-parentheses
LDFLAGS=-O2 -lm

SOURCES=$(wildcard *.cpp)
OBJECTS=$(addsuffix .o, $(basename ${SOURCES}))

EXECUTABLE=ZugzwangEngine

CFLAGS+=-g

build: $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

bin/%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ${EXECUTABLE} ${OBJECTS} *.d

.PHONY: all clean

