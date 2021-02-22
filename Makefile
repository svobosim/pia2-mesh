all: mesh

CPP = g++
CPPFLAGS = -std=c++11

mesh: 
	$(CPP) $(CPPFLAGS) -o mesh main.cpp Mesh.cpp

clean:	
	rm -f mesh
