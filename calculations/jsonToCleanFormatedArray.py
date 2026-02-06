import json
import math

import json
import math
import sys

class Logger:
    """Helper class to print to both console and a file."""
    def __init__(self, filename):
        self.terminal = sys.stdout
        self.log = open(filename, "w", encoding="utf-8")

    def write(self, message):
        self.terminal.write(message)
        self.log.write(message)

    def flush(self):
        # Needed for Python 3 compatibility
        self.terminal.flush()
        self.log.flush()

def normalize(v):
    """Returns the unit vector of the input vector v."""
    length = math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])
    if length == 0.0: return [0.0, 0.0, 0.0]
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
    s = f"{value:.6f}".rstrip('0')
    return s + "f"

def format_cpp_output(json_path, output_txt):
    # Set up dual-output logging
    sys.stdout = Logger(output_txt)

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
        bitang = [clean_float(x) for x in normalize([x * mult for x in raw_bitang])]
        
        processed_table.append([pos, texCoord, norm, tang, bitang])

    # Calculate column widths for perfect alignment
    col_widths = []
    for block_idx in range(5): # 5 blocks: Pos, TexCoord, Norm, Tang, Bitang
        num_elements = len(processed_table[0][block_idx])
        widths = [max(len(row[block_idx][i]) for row in processed_table) for i in range(num_elements)]
        col_widths.append(widths)

    # --- GENERATING OUTPUT ---
    print("    static const std::vector<Vertex> expectedVertices = {")
    
    # Header Calculation
    labels = ["POSITION", "TEX COORD", "NORMAL", "TANGENT", "BITANGENT"]
    # We start with 10 spaces to align with the first '{ { '
    header_line = " " * 10 
    
    for b_idx, label in enumerate(labels):
        # Calculate width of this specific block: "{ " + values + " }"
        block_width = sum(col_widths[b_idx]) + (len(col_widths[b_idx]) - 1) * 2 + 4
        header_line += "// " + label.ljust(block_width - 3)
        if b_idx < len(labels) - 1:
            header_line += "  " # Space for the ", " between blocks

    print(header_line.rstrip())

    for row in processed_table:
        line_parts = []
        for b_idx, block in enumerate(row):
            formatted_vals = [val.rjust(col_widths[b_idx][i]) for i, val in enumerate(block)]
            line_parts.append("{ " + ", ".join(formatted_vals) + " }")
        print("        { " + ", ".join(line_parts) + " },")
    
    print("    };\n")

    # ---- Process Indices ----
    print("    static const std::vector<unsigned int> expectedIndices = {")
    if indices_raw:
        idx_s = [str(i) for i in indices_raw]
        max_w = max(len(s) for s in idx_s)
        for i in range(0, len(idx_s), 3):
            chunk = ", ".join(n.rjust(max_w) for n in idx_s[i:i+3])
            print(f"        {chunk}{',' if i+3 < len(idx_s) else ''}")
    print("    };")

    # Restore standard stdout
    sys.stdout.log.close()
    sys.stdout = sys.stdout.terminal

# Run the tool
format_cpp_output("mesh.json", "output.txt")