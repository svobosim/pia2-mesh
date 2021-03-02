#include "Mesh.h"

// vypis souradnic bodu
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
};

// vypis geometrie polygonu
std::ostream& operator<<(std::ostream& os, const Polygon& p) {
	os << p.node_id.size();
	for (int j=p.node_id.size()-1; j>=0; j--) {
            os << " " << p.node_id[j];
        }
    return os;
};

Mesh::Mesh(double xl, double xr, double yl, double yr, int nx, int ny) {
	double dx = (xr - xl) / double(nx);
	double dy = (yr - yl) / double(ny);
	
	// nodes
	
	for(int i = 0; i<nx+1; ++i) {
		for(int j = 0; j < ny+1; ++j) {
			node.push_back({xl + i*dx, yl + j*dy});
		}
	}
	
	// cells
	
	for(int i = 0; i<nx; ++i) {
		for(int j = 0; j < ny; ++j) {
			cell.push_back(Polygon({i*(ny+1) + j+1,
			                        i*(ny+1) + j,
									(i+1)*(ny+1) + j,
									(i+1)*(ny+1) + j+1},*this)); //klic slovo - ukazatel na tento objekt pri vytvareni kazde bunky predam referenci na sit, 
									                             //abych se dostala treba k vrcholum
		}
	}	
	
}
