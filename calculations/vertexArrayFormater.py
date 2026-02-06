import re

input_data = """
        { { 0.f, -0.5f, 0.866025f }, { 0.5f, 1.f }, { 0.f, -1.f, 0.f }, { -1.f, -0.f, 0.f }, { 0.f, -0.f, 1.f } },
        { { 0.75f, -0.5f, -0.433013f }, { 0.f, 0.f }, { 0.f, -1.f, 0.f }, { -1.f, -0.f, 0.f }, { 0.f, -0.f, 1.f } },
        { { -0.75f, -0.5f, -0.433013f }, { 1.f, 0.f }, { 0.f, -1.f, 0.f }, { -1.f, -0.f, 0.f }, { 0.f, -0.f, 1.f } },
        { { 0.f, -0.5f, 0.866025f }, { 0.f, 1.f }, { 0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, -0.866025f }, { 0.288675f, -0.942809f, 0.166667f } },
        { { 0.75f, -0.5f, -0.433013f }, { 1.f, 1.f }, { 0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, -0.866025f }, { 0.288675f, -0.942809f, 0.166667f } },
        { { 0.f, 0.724745f, 0.f }, { 0.5f, 0.f }, { 0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, -0.866025f }, { 0.288675f, -0.942809f, 0.166667f } },
        { { 0.75f, -0.5f, -0.433013f }, { 0.f, 1.f }, { -0.f, 0.917663f, -0.39736f }, { -1.f, -0.f, 0.f }, { -0.f, -0.942809f, -0.333333f } },
        { { -0.75f, -0.5f, -0.433013f }, { 1.f, 1.f }, { -0.f, 0.917663f, -0.39736f }, { -1.f, -0.f, 0.f }, { -0.f, -0.942809f, -0.333333f } },
        { { 0.f, 0.724745f, 0.f }, { 0.5f, 0.f }, { -0.f, 0.917663f, -0.39736f }, { -1.f, -0.f, 0.f }, { -0.f, -0.942809f, -0.333333f } },
        { { -0.75f, -0.5f, -0.433013f }, { 0.f, 1.f }, { -0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, 0.866025f }, { -0.288675f, -0.942809f, 0.166667f } },
        { { 0.f, -0.5f, 0.866025f }, { 1.f, 1.f }, { -0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, 0.866025f }, { -0.288675f, -0.942809f, 0.166667f } },
        { { 0.f, 0.724745f, 0.f }, { 0.5f, 0.f }, { -0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, 0.866025f }, { -0.288675f, -0.942809f, 0.166667f } }
"""

def final_formatter(data):
    rows = re.findall(r'\{(.*)\},?', data)
    table = []
    for row in rows:
        sub_blocks = re.findall(r'\{(.*?)\}', row)
        parsed_blocks = [[val.strip() for val in b.split(',')] for b in sub_blocks]
        table.append(parsed_blocks)

    # Obliczanie max szerokości pod-kolumn
    col_widths = {}
    for row in table:
        for b_idx, block in enumerate(row):
            if b_idx not in col_widths: col_widths[b_idx] = [0] * len(block)
            for v_idx, val in enumerate(block):
                col_widths[b_idx][v_idx] = max(col_widths[b_idx][v_idx], len(val))

    formatted_lines = []
    block_positions = []

    for row in table:
        line_parts = []
        current_positions = []
        
        # Budujemy bazowy wiersz, by znać pozycje bloków
        current_line_str = "        { "
        for b_idx, block in enumerate(row):
            current_positions.append(len(current_line_str))
            formatted_vals = [val.rjust(col_widths[b_idx][v_idx]) for v_idx, val in enumerate(block)]
            block_str = "{ " + ", ".join(formatted_vals) + " }"
            current_line_str += block_str
            if b_idx < len(row) - 1:
                current_line_str += ", "
        
        current_line_str += " },"
        formatted_lines.append(current_line_str)
        if not block_positions: # Zapisujemy pozycje tylko raz
            block_positions = current_positions

    # Tworzenie linii komentarza
    labels = ["POSITION", "TEX COORD", "NORMAL", "TANGENT", "BITANGENT"]
    comment_line = list(" " * len(formatted_lines[0]))
    for pos, label in zip(block_positions, labels):
        comment_line[pos : pos + len("// " + label)] = "// " + label
    
    # Wyświetlanie
    print("    static const std::vector<Vertex> expectedVertices = {")
    print("".join(comment_line).rstrip())
    for line in formatted_lines:
        print(line)
    print("    };")

final_formatter(input_data)