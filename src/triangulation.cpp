#include "triangulation.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/boost/graph/helpers.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3                                     Point;
typedef CGAL::Surface_mesh<Point>                           Surface_mesh;
namespace PMP = CGAL::Polygon_mesh_processing;

void triangulateMesh(const std::string& inputFilePath, const std::string& outputFilePath) {
    Surface_mesh mesh;
    if(!PMP::IO::read_polygon_mesh(inputFilePath, mesh))
    {
        std::cerr << "Error: Invalid input." << std::endl;
        return;
    }
    if(is_empty(mesh))
    {
        std::cerr << "Warning: empty file?" << std::endl;
        return;
    }
    PMP::triangulate_faces(mesh);
    // Confirm that all faces are triangles.
    for(boost::graph_traits<Surface_mesh>::face_descriptor f : faces(mesh))
    {
        if(!CGAL::is_triangle(halfedge(f, mesh), mesh))
            std::cerr << "Error: non-triangular face left in mesh." << std::endl;
    }
    CGAL::IO::write_polygon_mesh(outputFilePath, mesh, CGAL::parameters::stream_precision(17));
}