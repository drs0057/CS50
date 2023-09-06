from cs50 import get_int

x = get_int("x: ")
y = get_int("y: ")

print(x + y)

#without cs50
x = int(input("x: ")) #otherwise the input will be considered a string (ASCII math)
y = int(input("y: "))

print(x + y)

z = x / y
print(f"{z:.50f}") #floating point imprecision