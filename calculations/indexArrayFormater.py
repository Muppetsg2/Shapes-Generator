import re

input_indices = """
        0, 1, 6,
        1, 2, 6,
        2, 3, 6,
        3, 4, 6,
        4, 5, 6,
        5, 0, 6,
        7, 14, 8,
        8, 14, 15,
        8, 15, 9,
        9, 15, 16,
        9, 16, 10,
        10, 16, 17,
        10, 17, 11,
        11, 17, 18,
        11, 18, 12,
        12, 18, 19,
        12, 19, 13,
        13, 19, 20,
        14, 21, 15,
        15, 21, 22,
        15, 22, 16,
        16, 22, 23,
        16, 23, 17,
        17, 23, 24,
        17, 24, 18,
        18, 24, 25,
        18, 25, 19,
        19, 25, 26,
        19, 26, 20,
        20, 26, 27,
        29, 28, 34,
        30, 29, 34,
        31, 30, 34,
        32, 31, 34,
        33, 32, 34,
        28, 33, 34
"""

def format_indices(data, indices_per_line=3):
    # Wyciągnij wszystkie liczby ze stringa
    numbers = re.findall(r'\d+', data)
    
    # Znajdź najdłuższą liczbę, aby ustalić szerokość kolumny
    max_len = max(len(n) for n in numbers)
    
    output = "    static const std::vector<unsigned int> expectedIndices = {\n"
    
    # Grupuj po 3 i formatuj
    for i in range(0, len(numbers), indices_per_line):
        chunk = numbers[i : i + indices_per_line]
        # Wyrównaj każdą liczbę do prawej na podstawie max_len
        formatted_chunk = ", ".join(n.rjust(max_len) for n in chunk)
        
        # Dodaj przecinek na końcu linii, jeśli to nie jest ostatnia grupa
        suffix = "," if i + indices_per_line < len(numbers) else ""
        output += f"        {formatted_chunk}{suffix}\n"
    
    output += "    };"
    return output

print(format_indices(input_indices))