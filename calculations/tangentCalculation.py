import math

def addVec3(a, b):
    return [
        a[0] + b[0],
        a[1] + b[1],
        a[2] + b[2]
    ]

def subVec3(a, b):
    return [
        a[0] - b[0],
        a[1] - b[1],
        a[2] - b[2]
    ]

def subVec2(a, b):
    return [
        a[0] - b[0],
        a[1] - b[1]
    ]

def mulVec3Float(a, b):
    return [
        a[0] * b,
        a[1] * b,
        a[2] * b
    ]

def lengthVec3(v):
    return math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])

def normalize(v):
    """Returns the unit vector of the input vector v."""
    length = lengthVec3(v)
    if length == 0.0:
        return [0.0, 0.0, 0.0]
    return [v[0] / length, v[1] / length, v[2] / length]

def cross(a, b):
    """Returns the cross product of vectors a and b."""
    return [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    ]

def dot(a, b):
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]

def gram_schmidt(t, n):
    return normalize(subVec3(t, mulVec3Float(n, dot(t, n))))

pos0 = [0.75, 0.144338, 0.0]
pos1 = [0.75, -0.144338, 0.0]
pos2 = [-0.375, 0.144338, 0.649519]

uv0 = [1.0, 0.333333]
uv1 = [0.0, 0.333333]
uv2 = [0.333333, 0.333333]

norm0 = [0.387298,  0.894427,  0.223607]
norm1 = [-0.612372,  0.707107, -0.353554]
norm2 = [-0.612372,  0.707107, -0.353554]

delta_pos1 = subVec3(pos1, pos0)
delta_pos2 = subVec3(pos2, pos0)

delta_uv1 = subVec2(uv1, uv0)
delta_uv2 = subVec2(uv2, uv0)

inv_r = delta_uv1[0] * delta_uv2[1] - delta_uv1[1] * delta_uv2[0]

if (abs(inv_r) >= 1e-6):
    r = 1.0 / inv_r if (abs(inv_r) >= 1e-6) else 1.0

    tangent = mulVec3Float(subVec3(mulVec3Float(delta_pos1, delta_uv2[1]), mulVec3Float(delta_pos2, delta_uv1[1])), r)
else:
    avg_normal = normalize(addVec3(norm0, addVec3(norm1, norm2)))

    up = [0.0, 1.0, 0.0] if abs(avg_normal[1]) < 0.999 else [1.0, 0.0, 0.0]

    tangent = cross(up, avg_normal)

# if (abs(lengthVec3(tangent)) >= 1e-6):
#    tangent = normalize(tangent)

print(f"Tangent = {tangent[0]:.6f}, {tangent[1]:.6f}, {tangent[2]:.6f}")

# Gram–Schmidt
tang0 = normalize(subVec3(tangent, mulVec3Float(norm0, dot(tangent, norm0))))
tang1 = normalize(subVec3(tangent, mulVec3Float(norm1, dot(tangent, norm1))))
tang2 = normalize(subVec3(tangent, mulVec3Float(norm2, dot(tangent, norm2))))

print(f"Tangent 0 = {tang0[0]:.6f}, {tang0[1]:.6f}, {tang0[2]:.6f}")
print(f"Tangent 1 = {tang1[0]:.6f}, {tang1[1]:.6f}, {tang1[2]:.6f}")
print(f"Tangent 2 = {tang2[0]:.6f}, {tang2[1]:.6f}, {tang2[2]:.6f}")