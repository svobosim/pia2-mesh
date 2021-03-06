#include "Mesh.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


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

//Vyhodi hranicni body (snad) libovolne nestrukturovane site (muze obsahovat i diry)
std::set<int> Mesh::returnBoundaryNodes(){
	//mnozina, zabranuje duplicite prvku
	std::set<int> boundaryNodes;
	//objekt hrany
	struct Edge{ //stejne jako class ale automaticky vse public
		Edge(int _n1, int _n2, double _hash) : n1(_n1), n2(_n2), hash(_hash) {};
		double hash;
		int n1, n2;
		//pretizeni operatoru < pro sort objektu
		bool operator< (const Edge &other) const {
			return hash < other.hash;
  }
	};
 //anonymni funkce - hash ktery kazde hrane priradi unikatni cislo
	auto hashFnc = [](double _x1, double _y1, double _x2, double _y2){
		double _hash;
		_hash = (_x1 + _x2)*14554 + _x1*_x2 + (_y1 + _y2)*137 + _y1*_y2*0.013;
		return _hash;
	};
	std::vector<Edge> edges;
	std::vector<Edge> edges_sorted;
	//vygenerovani jednotlivych hran -> nektere budou dvakrat
	for(auto &plg : cell){
		Polygon const& plgTemp = plg;
		int numVertices = plg.node_id.size();
		for(int i = 0; i < numVertices-1; i++){
			//ulozeni jednotlivych hran bunky
			edges.push_back(Edge(plg.node_id[i],	plg.node_id[i+1],
						hashFnc(node[plg.node_id[i]].x, node[plg.node_id[i]].y, node[plg.node_id[i+1]].x, node[plg.node_id[i+1]].y )));
		}
		//hrana ktera uzavira bunku
		edges.push_back(Edge(plg.node_id[numVertices-1], plg.node_id[0],
					hashFnc(node[plg.node_id[numVertices-1]].x, node[plg.node_id[numVertices-1]].y, node[plg.node_id[0]].x, node[plg.node_id[0]].y )));
	}
	//seradi hrany podle hodnoty hashe
	auto SortEdgesHash = [](std::vector<Edge> _edges){
		std::vector<Edge> edges_copy = _edges;
   std::sort(edges_copy.begin(), edges_copy.end());
   return edges_copy;
	};
	edges_sorted = SortEdgesHash(edges);
	//Vybere hrany na hranici a vyhodí jejich uzly (std::set odstrani duplicity)
	for(int i = 1; i < edges_sorted.size() - 1; i++){
		if(edges_sorted[i].hash != edges_sorted[i-1].hash && edges_sorted[i].hash != edges_sorted[i+1].hash){
			boundaryNodes.insert(edges_sorted[i].n1);
			boundaryNodes.insert(edges_sorted[i].n2);
		}
	}
	if(edges_sorted[edges_sorted.size()-1].hash != edges_sorted[edges_sorted.size()-2].hash){
			boundaryNodes.insert(edges_sorted[edges_sorted.size()-1].n1);
			boundaryNodes.insert(edges_sorted[edges_sorted.size()-1].n2);
	}
	if(edges_sorted[0].hash != edges_sorted[1].hash){
			boundaryNodes.insert(edges_sorted[0].n1);
			boundaryNodes.insert(edges_sorted[0].n2);
	}

	return boundaryNodes;
}
