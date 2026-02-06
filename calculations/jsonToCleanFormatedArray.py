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
    """Formats float: 1.456000 -> 1.456f, 1.000000 -> 1.f, 0.0 -> 0.f"""
    s = f"{value:.6f}"
    s = s.rstrip('0')
    if s.endswith('.'):
        return s + "f"
    return s + "f"

def format_cpp_output(json_path):
    # ---- Load JSON ----
    try:
        with open(json_path, "r") as f:
            data = json.load(f)
    except FileNotFoundError:
        print(f"Error: {json_path} not found.")
        return

    handedness = data.get("positiveHandedness", True)
    vertices_raw = data.get("vertices", [])
    indices_raw = data.get("indices", [])

    # ---- Process Vertices ----
    processed_table = []
    for v in vertices_raw:
        pos = [clean_float(x) for x in v.get("position", [0,0,0])]
        texCoord = [clean_float(x) for x in v.get("texCoord", [0,0])]
        norm = [clean_float(x) for x in v.get("normal", [0,0,0])]
        tang = [clean_float(x) for x in v.get("tangent", [0,0,0])]
        
        # Calculate Bitangent
        raw_bitang = cross(v.get("normal", [0,0,0]), v.get("tangent", [0,0,0]))
        mult = 1.0 if handedness is True else -1.0
        bitang_vec = normalize([x * mult for x in raw_bitang])
        bitang = [clean_float(x) for x in bitang_vec]
        
        processed_table.append([pos, texCoord, norm, tang, bitang])

    # Calculate column widths for perfect alignment
    col_widths = []
    for block_idx in range(5): # 5 blocks: Pos, TexCoord, Norm, Tang, Bitang
        num_elements = len(processed_table[0][block_idx])
        widths = [0] * num_elements
        for row in processed_table:
            for i, val in enumerate(row[block_idx]):
                widths[i] = max(widths[i], len(val))
        col_widths.append(widths)

    # ---- Generate Vertices String ----
    vert_output = "    static const std::vector<Vertex> expectedVertices = {\n"
    
    # Headers with dynamic spacing
    labels = ["POSITION", "TEX COORD", "NORMAL", "TANGENT", "BITANGENT"]
    header_line = "        "
    for b_idx, label in enumerate(labels):
        # Create a dummy formatted block to calculate length
        sample_vals = [" " * col_widths[b_idx][i] for i in range(len(col_widths[b_idx]))]
        block_str = "{ " + ", ".join(sample_vals) + " }"
        header_line += "// " + label.ljust(len(block_str) - 3) + "  "
    
    vert_output += header_line.rstrip() + "\n"

    for row in processed_table:
        line_parts = []
        for b_idx, block in enumerate(row):
            formatted_vals = [val.rjust(col_widths[b_idx][i]) for i, val in enumerate(block)]
            line_parts.append("{ " + ", ".join(formatted_vals) + " }")
        vert_output += "        { " + ", ".join(line_parts) + " },\n"
    vert_output += "    };\n"

    # ---- Process Indices ----
    indices_output = "\n    static const std::vector<unsigned int> expectedIndices = {\n"
    if indices_raw:
        idx_strings = [str(i) for i in indices_raw]
        max_idx_len = max(len(s) for s in idx_strings)
        for i in range(0, len(idx_strings), 3):
            chunk = idx_strings[i:i+3]
            formatted_chunk = ", ".join(n.rjust(max_idx_len) for n in chunk)
            comma = "," if i + 3 < len(idx_strings) else ""
            indices_output += f"        {formatted_chunk}{comma}\n"
    indices_output += "    };"

    print(vert_output + indices_output)

# Run the tool
format_cpp_output("mesh.json")