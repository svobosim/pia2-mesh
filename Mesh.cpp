#include "Mesh.h"
#include <iostream>
#include <vector>
#include <cmath>
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << " " << p.y << " 0.0";
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
    double dx = (xr - xl)/double(nx);
    double dy = (yr - yl)/double(ny);
    
    // Create nodes
    for (int i=0; i<nx+1; ++i) {
        for (int j=0; j<ny+1; ++j) {
            node.push_back({xl + i*dx, yl + j*dy});
        }
    }
    
    // Create cells
    for (int i=0; i<nx; ++i) {
        for (int j=0; j<ny; ++j) {
            cell.push_back(Polygon({i*(ny+1) + j+1,
                                    i*(ny+1) + j,
                                    (i+1)*(ny+1) + j,
                                    (i+1)*(ny+1) + j + 1},*this));
        }
    }
};

std::vector<int> Mesh::pointCellNeighbors(int p){
	std::vector<int> pointCellNeighbors;
	
	if (p < 0 || p >= node.size()){
		std::cout << "Warning: selected point does not exist.";
	}
	
	for(int i=0; i<cell.size();i++){
		Polygon const& polygonTmp = cell[i];
		for(int j=0; j<polygonTmp.node_id.size(); j++){
			if (polygonTmp.node_id[j]==p){
				pointCellNeighbors.push_back(i);
			}
		}
	}
	return pointCellNeighbors;
};

double Polygon::area(){
	double plocha, lsum, rsum;
	for (int j=0; j<(node_id.size()-1.0); ++j) {                                   // potreuju cyklus od 0 do poctu nodu meho polygonu-1
		lsum = lsum + mesh.node[node_id[j]].x * mesh.node[node_id[j+1]].y;
		rsum = rsum + mesh.node[node_id[j+1]].x * mesh.node[node_id[j]].y;
	}
	plocha = std::abs (lsum + mesh.node[node_id[node_id.size()-1.0]].x * mesh.node[node_id[0]].y) - rsum - (mesh.node[node_id[0]].x * mesh.node[node_id[node_id.size()-1.0]].y);
	plocha = plocha*0.5;
	return plocha;
}

//test konvexnosti bunky (1 = je konvexni; 0 = neni konvexni)
bool Polygon::isConvex(){
	bool isConvex;
	double u11, u12, u21, u22, v11, v12, v21, v22, u31, u32, v31, v32, w13, w23, w33; 
	//prvni index = 1 - tyka se cyklu pres vsechny uzly krome poslednich dvou
	//prvni index = 2 - tyka se predposledniho uzlu
	//prvni index = 3 - tyka se posledniho uzlu
	//druhy index - slozka prislusneho vektoru
	
//cyklus pres vsechny uzly krome poslednich dvou
			for(int j=0; j<(node_id.size()-2); ++j){
			u11 = mesh.node[node_id[j+1]].x - mesh.node[node_id[j]].x;
			u12 = mesh.node[node_id[j+1]].y - mesh.node[node_id[j]].y;
			
			v11 = mesh.node[node_id[j+2]].x - mesh.node[node_id[j]].x;
			v12 = mesh.node[node_id[j+2]].y - mesh.node[node_id[j]].y;
			
			w13 = u11 * v12 - v11 * u12;
	    	}

//vektory u,v vedene z predposledniho uzlu   	
			u21 = mesh.node[node_id[node_id.size()]].x - mesh.node[node_id[node_id.size()-1]].x;
			u22 = mesh.node[node_id[node_id.size()]].y - mesh.node[node_id[node_id.size()-1]].y;
			
			v21 = mesh.node[node_id[0]].x - mesh.node[node_id[node_id.size()-1]].x;
			v22 = mesh.node[node_id[0]].y - mesh.node[node_id[node_id.size()-1]].y;
			
			w23 = u21 * v22 - v21 * u22;

//vektory u,v vedene z posledniho uzlu 			
			u31 = mesh.node[node_id[0]].x - mesh.node[node_id[node_id.size()]].x;
			u32 = mesh.node[node_id[0]].y - mesh.node[node_id[node_id.size()]].y;
			
			v31 = mesh.node[node_id[1]].x - mesh.node[node_id[node_id.size()]].x;
			v32 = mesh.node[node_id[1]].y - mesh.node[node_id[node_id.size()]].y;
			
			w33 = u31 * v32 - v31 * u32;
	    
		if((w13 <= 0) && (w23 <= 0) && (w33 <= 0)){
	    	isConvex = 0;
		}else{	
			isConvex = 1;
		};

return isConvex;
}
