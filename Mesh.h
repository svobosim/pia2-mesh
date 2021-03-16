#ifndef MESH_H
#define MESH_H

#include <vector>
#include <sstream>
#include <set>
#include <cmath>

class Mesh; // Forward declaration required for class Polygon to know
            // about class Mesh.

class Point {
public:
    Point(double _x, double _y) : x(_x), y(_y) {};
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
    double x, y;
};

class Polygon {
public:
    Polygon(std::vector<int> const& _node_id, Mesh & _m) : node_id(_node_id), mesh(_m) {};
    friend std::ostream& operator<<(std::ostream& os, const Polygon& p);
    std::vector<int> node_id;
    Mesh& mesh;
    double area();  // trida polygon obsahuje metodu area
    bool isConvex();
    double edgeLength(int i); 
};

struct Edge{ //stejne jako class ale automaticky vse public
	Edge(int _n1, int _n2, double _hash) : n1(_n1), n2(_n2), hash(_hash) {};
	double hash;
	int n1, n2;
	//pretizeni operatoru < pro sort objektu, == pro std::unique
	bool operator< (const Edge &other) const { return hash < other.hash; };
  bool operator== (const Edge &other) const { return std::fabs(hash - other.hash) < 1.0e-8; };
	// Vector2D normal();
	// double length();
	// int left();
	// int right();
};

class Mesh {
public:
    Mesh(double xl, double xr, double yl, double yr, int nx, int ny);
    std::vector<Polygon> cell;
    std::vector<Point> node;
    std::vector<Edge> edge;
    std::vector<int> pointCellNeighbors(int p);
    void generateEdges();
    std::set<int> boundaryNodes;
    void randomize(double r);
};

#endif //MESH_H

