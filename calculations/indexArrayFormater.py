import re

input_indices = """
        0, 2, 1,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11
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