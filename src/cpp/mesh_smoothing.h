#ifndef MESH_SMOOTHING_H
#define MESH_SMOOTHING_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> 
#include <CGAL/Surface_mesh.h>
#include <string>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;

Mesh smoothShape(const Mesh& inputMesh, unsigned int nb_iterations = 25, double time = 0.005);

#endif // MESH_SMOOTHING_H
