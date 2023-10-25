import numpy as np
import plotly.graph_objects as go
import plotly.figure_factory as ff

# Step 1: Generate random 3D vectors
np.random.seed(0)  # for reproducibility
n_vectors = 3
vectors = 2 * np.random.rand(n_vectors, 3) - 1

# Checkpoint 1: Visualize the generated 3D vectors
fig = go.Figure()
for vector in vectors:
    fig.add_trace(go.Scatter3d(
        x=[0, vector[0]],
        y=[0, vector[1]],
        z=[0, vector[2]],
        mode='lines',
        line=dict(color='blue', width=6)
    ))
fig.update_layout(title='Generated 3D Vectors')
fig.show()

# Step 2: Convert Cartesian coordinates to spherical coordinates
rho, phi, theta = (
    np.linalg.norm(vectors, axis=1), 
    np.arctan2(vectors[:, 1], vectors[:, 0]), 
    np.arccos(np.clip(vectors[:, 2]/np.linalg.norm(vectors, axis=1), -1, 1))
)

# Checkpoint 2: Visualize spherical coordinates as a scatter plot
fig = go.Figure()
fig.add_trace(go.Scatter3d(
    x=rho * np.sin(theta) * np.cos(phi),
    y=rho * np.sin(theta) * np.sin(phi),
    z=rho * np.cos(theta),
    mode='markers',
    marker=dict(color='red', size=5)
))
fig.update_layout(title='Spherical Coordinates Scatter Plot')
fig.show()








# Step 3: Define the Gaussian function

# ONLY DIRECTIONS (MAGNITUDES DISREGARDED)
def gaussian(vec_dir, sigma, theta, phi):
    # vec_dir is now a tuple of (rho, theta, phi) for a single vector
    # Compute the angular difference based on the spherical coordinates
    angular_difference = np.arccos(
        np.sin(theta) * np.sin(vec_dir[1]) * np.cos(phi - vec_dir[2]) +
        np.cos(theta) * np.cos(vec_dir[1])
    )
    return np.exp(-(angular_difference ** 2) / (2 * sigma ** 2))


# DIRECTIONS AND MAGNITUDES
def gaussian(vec_dir, sigma, theta, phi):
    delta_theta = theta - vec_dir[1]
    delta_phi = phi - vec_dir[2]
    # Multiply by vec_dir[0] to scale by the vector's magnitude
    return vec_dir[0] * np.exp(-(np.arccos(np.sin(theta) * np.sin(vec_dir[1]) * np.cos(delta_phi) + np.cos(theta) * np.cos(vec_dir[1])) ** 2) / (2 * sigma ** 2))


# Step 4: Redefine the distribution function
def distribution_function(theta, phi, sigma=0.5):
    # Sum the Gaussian values for all vectors at the given theta and phi
    return np.sum([gaussian((rho[i], theta_vec[i], phi_vec[i]), sigma, theta, phi) for i in range(n_vectors)])

# Spherical coordinate arrays for the vectors
theta_vec = theta
phi_vec = phi

# Checkpoint 3 (a): Visualize the distribution function on a grid of theta and phi values
theta_grid, phi_grid = np.meshgrid(np.linspace(0, np.pi, 100), np.linspace(0, 2 * np.pi, 100))
distribution_values = np.vectorize(distribution_function)(theta_grid, phi_grid)

# Convert spherical to Cartesian coordinates for visualization
x = np.sin(theta_grid) * np.cos(phi_grid)
y = np.sin(theta_grid) * np.sin(phi_grid)
z = np.cos(theta_grid)

fig = go.Figure(data=[go.Surface(z=z, x=x, y=y, surfacecolor=distribution_values, colorscale='Viridis')])
fig.update_layout(title='Distribution Function on Sphere', scene=dict(xaxis_title='X', yaxis_title='Y', zaxis_title='Z'))
fig.show()



#  Checkpoint 3 (b): Visualize the distribution function on a grid of theta and phi values
theta_grid, phi_grid = np.meshgrid(np.linspace(0, np.pi, 100), np.linspace(0, 2 * np.pi, 100))
distribution_values = np.vectorize(distribution_function)(theta_grid, phi_grid)

# Convert spherical to Cartesian coordinates for visualization
x = np.sin(theta_grid) * np.cos(phi_grid)
y = np.sin(theta_grid) * np.sin(phi_grid)
z = np.cos(theta_grid)

# Create a figure
fig = go.Figure()

# Add the surface plot (with some transparency)
fig.add_trace(go.Surface(
    z=z, x=x, y=y,
    surfacecolor=distribution_values,
    colorscale='Viridis',
    opacity=0.9  # Adjust the opacity to allow for some transparency
))

# Add the original vectors
for vector in vectors:
    fig.add_trace(go.Scatter3d(
        x=[0, vector[0]],
        y=[0, vector[1]],
        z=[0, vector[2]],
        mode='lines',
        line=dict(color='blue', width=6)
    ))

fig.update_layout(title='Distribution Function on Sphere with Original Vectors', scene=dict(xaxis_title='X', yaxis_title='Y', zaxis_title='Z'))
fig.show()










