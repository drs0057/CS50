import sys

names = ["Bill", "Charlie", "Fred", "George", "Ginny", "Percy", "Ron"]

name = input("Name: ")

for n in names:
    if name == n:
        print("Found")
        sys.exit(0)

print("Not found")

#easier way

names = ["Bill", "Charlie", "Fred", "George", "Ginny", "Percy", "Ron"]

name = input("Name: ")

if name in names: #python does the linear search
    print("Found")
    sys.exit(0)

print("Not found")