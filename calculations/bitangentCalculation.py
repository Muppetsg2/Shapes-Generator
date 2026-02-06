import json
import math

def normalize(v):
    """Returns the unit vector of the input vector v."""
    length = math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])
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

def clean_float(value):
    """Formatting: 1.456000 -> 1.456f, 1.000000 -> 1.f"""
    s = f"{value:.6f}"
    s = s.rstrip('0')  # Remove trailing zeros
    if s.endswith('.'):
        return s + "f" # Convert "1." to "1.f"
    return s + "f"

# ---- load json ----
# mesh.json should contain: {"vertices": [{"normal": [x,y,z], "tangent": [x,y,z]}], "positiveHandedness": true}
try:
    with open("mesh.json", "r") as f:
        data = json.load(f)
except FileNotFoundError:
    # Fallback data if file is missing
    data = {"vertices": []}

# Global handedness from the root of the JSON
handedness = data.get("positiveHandedness", True)
vertices = data.get("vertices", [])

print(f"(Handedness: {handedness})")

for i, v in enumerate(vertices):
    normal = v.get("normal")
    tangent = v.get("tangent")

    if normal is None or tangent is None:
        print(f"[{i}] normal or tangent is empty")
        continue

    # Calculate bitangent
    raw_bitangent = cross(normal, tangent)
    
    # Apply handedness (right/left handed coordinate system)
    multiplier = 1.0 if handedness is True else -1.0
    bitangent = [val * multiplier for val in raw_bitangent]
    bitangent = normalize(bitangent)

    # Format numbers for output
    v_strings = [clean_float(x) for x in bitangent]
    
    # Print in the requested format
    # Using rjust to align numbers and prevent minus signs from shifting brackets
    formatted_vals = f"{v_strings[0].rjust(10)}, {v_strings[1].rjust(10)}, {v_strings[2].rjust(10)}"
    print(f"bitangent[{i}] = {{ {formatted_vals} }}")