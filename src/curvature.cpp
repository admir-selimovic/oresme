#include <igl/read_triangle_mesh.h>
#include <igl/principal_curvature.h>
#include <Eigen/Core>
#include <iostream>
#include <string>
#include <fstream>


void writeCSV(const Eigen::MatrixXd& PD1, const Eigen::MatrixXd& PD2, const Eigen::VectorXd& PV1, const Eigen::VectorXd& PV2, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    file << "PD1_x,PD1_y,PD1_z,PD2_x,PD2_y,PD2_z,PV1,PV2\n";  // Write header

    for (int i = 0; i < PD1.rows(); ++i) {
        for (int j = 0; j < PD1.cols(); ++j) {
            file << PD1(i, j);
            if (j < PD1.cols() - 1)
                file << ",";
        }

        file << ",";

        for (int j = 0; j < PD2.cols(); ++j) {
            file << PD2(i, j);
            if (j < PD2.cols() - 1)
                file << ",";
        }

        file << "," << PV1(i) << "," << PV2(i) << "\n";
    }

    file.close();
}



void calculateCurvature(const std::string& mesh_file, const std::string& output_file) {
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    bool success = igl::read_triangle_mesh(mesh_file, V, F);

    // Print whether the mesh was loaded successfully
    if(success) {
        std::cout << "Mesh loaded successfully!" << std::endl;
        std::cout << "Number of vertices: " << V.rows() << std::endl;
        std::cout << "Number of faces: " << F.rows() << std::endl;
    } else {
        std::cout << "Failed to load mesh." << std::endl;
        return;  // Exit function if mesh loading failed
    }

    Eigen::MatrixXd PD1, PD2;
    Eigen::VectorXd PV1, PV2;
    try {
        igl::principal_curvature(V, F, PD1, PD2, PV1, PV2);
        std::cout << "Principal curvature calculation completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
    }

    // Print a success message if principal curvature calculation completes
    std::cout << "Principal curvature calculation completed successfully!" << std::endl;

    writeCSV(PD1, PD2, PV1, PV2, output_file);

}