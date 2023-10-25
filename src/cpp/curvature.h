#ifndef CURVATURE_H
#define CURVATURE_H

#include <Eigen/Core>
#include <string>

// Function to calculate and write the principal curvature to a CSV file
void calculateCurvature(const std::string& mesh_file, const std::string& output_file);

#endif // CURVATURE_H