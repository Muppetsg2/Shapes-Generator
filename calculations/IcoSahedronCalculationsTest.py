print("VERTICES:")
t = 2
for i in range(4):
    a = t * (1 - 2 * (int)(i / 2))
    b =  1 - 2 * ((i + 1) % 2)
    print(f"{b}, {a}")

arr = []

print("\nINDICES:")
for i in range(5):
    x = 0
    y = (11 + (i - ((int)((i + 2 * ((int)((i + 2) / 4) % 2)) / 2) % 2)) * 6 + (int)(i / 2) * 2 + (int)(i / 4)) % 12
    z = (5 + 8 * ((((((i + 1) % 3) % 2) - (int)((i % 4) / 3)) + 1) % 2) + 3 * ((i - 1) * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) + 2 * ((int)(i / 2) % 2) + ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) % 12
    print(f"{x}, {y}, {z}")
    arr.append(x)
    arr.append(y)
    arr.append(z)

print("")
for i in range(5):
    x = (1 + 2 * (2 + 3 * ((i - 1) * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2))) * (((((i + 1) % 3) % 2) - (int)((i % 4) / 3) + 1) % 2) + 3 * ((i - 2) * ((int)((i % 4) / 3) + ((int)((i + 2 * ((int)((i + 2) / 4) % 2)) / 2) % 2))) + 2 * ((int)((i % 4) / 3) + ((int)((i + 2 * ((int)((i + 2) / 4) % 2)) / 2) % 2))) % 12
    y = (5 + 6 * i + 3 * (((int)(i / 3) + 1) * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) + 2 * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) % 12
    z = (9 + 7 * (((((i + 1) % 3) % 2 - (int)((i % 4) / 3)) + 1) % 2) + 2 * (2 * i + 1 * ((int)((i % 4) / 3) + ((int)((i + ((int)((i + 1) / 4) % 2)) / 2) % 2))) * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) % 12
    print(f"{x}, {y}, {z}")
    arr.append(x)
    arr.append(y)
    arr.append(z)

print("")
for i in range(5):
    x = 3
    y = (9 + 7 * (((((i + 1) % 3) % 2 - (int)((i % 4) / 3)) + 1) % 2) + 2 * (2 * i + 1 * ((int)((i % 4) / 3) + ((int)((i + ((int)((i + 1) / 4) % 2)) / 2) % 2))) * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) % 12
    z = (4 + 2 * (5 + i * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) * (((((i + 1) % 3) % 2 - (int)((i % 4) / 3)) + 1) % 2) - ((int)((i + 2 * ((int)((i + 2) / 4) % 2)) / 2) % 2)) % 12
    print(f"{x}, {y}, {z}")
    arr.append(x)
    arr.append(y)
    arr.append(z)

print("")
for i in range(5):
    x = (4 + 2 * (5 + i * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) * (((((i + 1) % 3) % 2 - (int)((i % 4) / 3)) + 1) % 2) - ((int)((i + 2 * ((int)((i + 2) / 4) % 2)) / 2) % 2)) % 12
    y = (9 + 7 * (((((i + 1) % 3) % 2 - (int)((i % 4) / 3)) + 1) % 2) + 2 * (2 * i + 1 * ((int)((i % 4) / 3) + ((int)((i + ((int)((i + 1) / 4) % 2)) / 2) % 2))) * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) % 12
    z = (5 + 6 * i + 3 * (((int)(i / 3) + 1) * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) + 2 * ((int)((i + 2 * ((int)(i / 4) % 2)) / 2) % 2)) % 12
    print(f"{x}, {y}, {z}")
    arr.append(x)
    arr.append(y)
    arr.append(z)

print("\nNUMBER OF TRIANGLES PER VERTEX:")
for i in range(12):
    print(f"{i}: {arr.count(i)}")