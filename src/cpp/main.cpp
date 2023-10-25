#include "triangulation.h"
#include "isotropic_remeshing.h"
#include "mesh_smoothing.h"
#include "curvature.h"
#include <iostream>
#include <cstdio>
#include <boost/filesystem.hpp>
#include <string>

namespace fs = boost::filesystem;

// Function to process a single file
void processFile(const fs::path& filePath) {
    // Define the file path for the temporary triangulated mesh
    const std::string tempTriangulatedFilePath = (filePath.parent_path() / "temp_triangulated.obj").string();
    
    // Call the triangulateMesh function to triangulate the mesh and save it to tempTriangulatedFilePath
    triangulateMesh(filePath.string(), tempTriangulatedFilePath);

    double target_edge_length = 1.0;  // Assuming a default value
    unsigned int nb_iter = 10;  // Assuming a default value
    
    // Now pass the tempTriangulatedFilePath to processMesh function
    Mesh remeshed = processMesh(tempTriangulatedFilePath, target_edge_length, nb_iter);
    
    Mesh smoothedMesh = smoothShape(remeshed, 25, 0.005);
    
    // Exporting the remeshed object to a file
    const std::string outputFilePath = (filePath.parent_path() / "mesh_isotropic_remeshing.obj").string();
    CGAL::IO::write_polygon_mesh(outputFilePath, remeshed, CGAL::parameters::stream_precision(17));

    // Exporting the smoothed object to a file
    const std::string outputFilePathSmoothed = (filePath.parent_path() / "mesh_shape_smoothed.obj").string();
    CGAL::IO::write_polygon_mesh(outputFilePathSmoothed, smoothedMesh, CGAL::parameters::stream_precision(17));
    
    // Now call the calculateCurvature function to process the smoothed mesh
    const std::string outputCurvatureFile = (filePath.parent_path() / "curvature.csv").string();
    calculateCurvature(outputFilePathSmoothed, outputCurvatureFile);
    
    // Delete the exported .obj files
    //if (remove(outputFilePath.c_str()) != 0) {
    //    std::perror(("Error deleting the remeshed object file at " + outputFilePath).c_str());
    //}
    //if (remove(outputFilePathSmoothed.c_str()) != 0) {
    //    std::perror(("Error deleting the smoothed object file at " + outputFilePathSmoothed).c_str());
    //}
    // Delete the temporary triangulated mesh file
    if (remove(tempTriangulatedFilePath.c_str()) != 0) {
        std::perror(("Error deleting the temporary triangulated mesh file at " + tempTriangulatedFilePath).c_str());
    }
}

// Recursive function to traverse the directory structure and find files to process
void traverseAndProcess(const fs::path& dirPath) {
    for (fs::directory_iterator it(dirPath); it != fs::directory_iterator(); ++it) {
        if (fs::is_directory(it->status())) {
            // Recurse into subdirectories
            traverseAndProcess(it->path());
        } else if (fs::is_regular_file(it->status())) {
            // Check if the file name contains "LOD0" and has .obj extension
            std::string fileName = it->path().filename().string();
            if (fileName.find("LOD0") != std::string::npos && it->path().extension() == ".obj") {
                processFile(it->path());
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // Get the path of the main folder from the command line or use a default value
    const fs::path mainFolder = (argc > 1) ? argv[1] : "C:/Users/Admir Selimovic/glypta/data/raw";
    traverseAndProcess(mainFolder);
    return 0;
}