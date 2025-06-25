from cs50 import get_int

# PROMPT THE USER FOR THE HEIGHT OF THE PYRAMID
height = get_int('Height: ')

# ENSURE THAT THE HEIGHT IS BETWEEN 1 ND 8
while (height < 1 or height > 8):
    height = get_int('Height: ')

for i in range(height, 0, -1):
    # PRINT SPACES BEFORE THE LEFT PYRAMID
    for j in range(height, height - i + 1, -1):
        print(' ', end='')

    # PRINT THE RIGHT PYRAMID
    for j in range(height - i + 1):
        print('#', end='')

    # PRINT THE GAP IN BETWEEN
    print('  ', end='')

    # PRINT THE LEFT PYRAMID
    for j in range(height - i + 1):
        print('#', end='')

    print()  # PRINT A NEW LINE
