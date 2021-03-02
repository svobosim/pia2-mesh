#ifndef OUTPUT_H
#define OUTPUT_H

#include "Mesh.h"
#include "Field.h"
#include <string>

void outputVTK(std::string filename, Mesh const& m, Field<double> const& u);

#endif //OUTPUT_H