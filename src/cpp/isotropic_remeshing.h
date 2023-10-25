#ifndef ISOTROPIC_REMESHING_H
#define ISOTROPIC_REMESHING_H

#include <CGAL/Surface_mesh.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <string>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;

Mesh processMesh(const std::string& inputFilePath, double target_edge_length, unsigned int nb_iter);
//Mesh processMesh(const std::string& inputFilePath, double target_edge_length = 1.0, unsigned int nb_iter = 10);

#endif // ISOTROPIC_REMESHING_H