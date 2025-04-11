import numpy as np
import matplotlib.pyplot as plt

####################
#     CONE UV     #
####################

def cone_uv_circle_points(circle_center, theta_degrees):
    # Convert degrees to radians
    theta = np.radians(theta_degrees)
    
    # Circle data
    x_s, y_s = circle_center  # Circle center
    r = np.sqrt(2) / 2  # Radius
    theta_0 = 3 * np.pi / 4  # Shift angle (135 degrees)
    
    # Parametric equations
    x = x_s + r * np.cos(theta_0 - theta)
    y = y_s + r * np.sin(theta_0 - theta) + 0.25
    
    return x, y

def cone_uv_circle_points_2(circle_center, theta_degrees):
    # Convert degrees to radians
    theta = np.radians(theta_degrees)

    # Circle data
    x_s, y_s = circle_center  # Circle center
    theta_0 = np.pi / 6.0 # Shift angle

    # Parametric equations
    x = x_s + np.sin(theta - theta_0)
    y = y_s + np.cos(theta - theta_0)

    return x, y

# Testing for angles 0, 15, 30, 45, 60, 75, 90 degrees for cone_uv_circle_points
# test_angles = [0, 15, 30, 45, 60, 75, 90]

# Testing for angles 0, 10, 20, 30, 40, 50, 60 degrees for cone_uv_circle_points_2
test_angles = [0, 10, 20, 30, 40, 50, 60]

circle_center = (0.5, 0)

points = []
points.append(circle_center)
for angle in test_angles:
    # x, y = cone_uv_circle_points(circle_center, angle)
    x, y = cone_uv_circle_points_2(circle_center, angle)
    points.append((x, y))
    print(f"Theta = {angle}° -> x = {x:.4f}, y = {y:.4f}")

points.append(circle_center)

# Rozdzielenie współrzędnych do list
xs, ys = zip(*points)

# Tworzenie wykresu
plt.figure(figsize=(6, 6))
plt.plot(xs, ys, '-o', label='Merge Points')

# Dodanie numerów kroków
for i, (x, y) in enumerate(points):
    plt.text(x, y, f'{i}', fontsize=9, ha='right', va='bottom')

# Ograniczenie osi do zakresu [0,1]
plt.xlim(0, 1)
plt.ylim(0, 1)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Cone UV points')
plt.grid(True)
plt.gca().set_aspect('equal', adjustable='box')
plt.legend()
plt.show()

####################
# CONE FACE NORMAL #
####################

def cross_product(vec1, vec2):
    # Unpack vector coordinates
    x1, y1, z1 = vec1
    x2, y2, z2 = vec2
    
    # Compute cross product
    cross_prod = [
        y1 * z2 - z1 * y2,  # x component
        z1 * x2 - x1 * z2,  # y component
        x1 * y2 - y1 * x2   # z component
    ]
    
    return cross_prod

def subtract_vectors(vec1, vec2):
    # Compute coordinate differences
    result = [
        vec1[0] - vec2[0],  # x component
        vec1[1] - vec2[1],  # y component
        vec1[2] - vec2[2]   # z component
    ]
    
    return result

def normalize_vector(vector):
    vector = np.array(vector)
    norm = np.linalg.norm(vector)
    
    if norm == 0:
        return vector
    
    return vector / norm

#        /|
#   c   / | h
#      /  |
#     /___|
#       r

r = [0.5, 0, 0]
h = [0, 3.0, 0]
v = normalize_vector(cross_product(subtract_vectors(h, r), cross_product(r, h)))
print("Vector perpendicular to the hypotenuse of the triangle:", v)

# Plot the triangle
fig, ax = plt.subplots()
ax.set_xlim(-1, 1)
ax.set_ylim(-0.5, 3.5)
ax.set_aspect('equal')

# Define triangle points
triangle_points = np.array([[0, 0], r[:2], h[:2], [0, 0]])
ax.plot(triangle_points[:, 0], triangle_points[:, 1], 'bo-', label='Triangle')

# Plot perpendicular vector v
v_start = [(r[0] + h[0]) / 2, (r[1] + h[1]) / 2]  # Midpoint of hypotenuse
v_end = [v_start[0] + v[0] * 0.2, v_start[1] + v[1] * 0.2]  # Scaled for visualization
ax.arrow(v_start[0], v_start[1], v_end[0] - v_start[0], v_end[1] - v_start[1], 
         head_width=0.1, head_length=0.1, fc='r', ec='r', label='Perpendicular vector v')

# Labels
ax.text(*r[:2], ' r', fontsize=12, verticalalignment='bottom', horizontalalignment='left')
ax.text(*h[:2], ' h', fontsize=12, verticalalignment='bottom', horizontalalignment='right')
ax.text(*v_end, ' v', fontsize=12, color='r')

ax.legend()
plt.grid()
plt.show()
