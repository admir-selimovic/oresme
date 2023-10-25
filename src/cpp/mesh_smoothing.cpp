#include "mesh_smoothing.h"
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <iostream>
#include <set>

namespace PMP = CGAL::Polygon_mesh_processing;

Mesh smoothShape(const Mesh& inputMesh, unsigned int nb_iterations, double time) {
    Mesh mesh = inputMesh;  // Make a copy of the input mesh to work on
    std::set<Mesh::Vertex_index> constrained_vertices;
    for(Mesh::Vertex_index v : vertices(mesh)) {
        if(is_border(v, mesh))
            constrained_vertices.insert(v);
    }
    std::cout << "Constraining: " << constrained_vertices.size() << " border vertices" << std::endl;
    CGAL::Boolean_property_map<std::set<Mesh::Vertex_index>> vcmap(constrained_vertices);
    std::cout << "Smoothing shape... (" << nb_iterations << " iterations)" << std::endl;
    PMP::smooth_shape(mesh, time, CGAL::parameters::number_of_iterations(nb_iterations)
                                              .vertex_is_constrained_map(vcmap));
    std::cout << "Smoothing done!" << std::endl;
    return mesh;  // Return the smoothed mesh
}
