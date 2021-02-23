all: mesh

CPP = g++
CPPFLAGS = -std=c++11

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

mesh: main.o Mesh.o
	$(CPP) $(CPPFLAGS) -o mesh main.o Mesh.o

clean:	
	rm -f *.o mesh
