import json
import math

def normalize(v):
    length = math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])
    if length == 0.0:
        return [0.0, 0.0, 0.0]
    return [v[0] / length, v[1] / length, v[2] / length]

def cross(a, b):
    return [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    ]

# ---- load json ----
with open("mesh.json", "r") as f:
    data = json.load(f)

handedness = data.get("positiveHandedness", True)
vertices = data["vertices"]

for i, v in enumerate(vertices):
    normal = v.get("normal")
    tangent = v.get("tangent")

    if normal is None or tangent is None:
        print(f"[{i}] normal or tangent is empty")
        continue

    bitangent = cross(normal, tangent)

    multiplier = 1.0 if handedness is True else -1.0

    bitangent = [
        bitangent[0] * multiplier,
        bitangent[1] * multiplier,
        bitangent[2] * multiplier
    ]

    bitangent = normalize(bitangent)

    print(
        f"[{i}] "
        f"bitangent = "
        f"({bitangent[0]: .6f}, {bitangent[1]: .6f}, {bitangent[2]: .6f}) "
        f"(Handedness: {handedness})"
    )