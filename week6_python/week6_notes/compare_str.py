s = input("s: ")
t = input("t: ")

if s == t: #unlike C, actually compares strings char by char for equality
    print("Same")
else:
    print("Different")

#or
s = input("s: ")
t = s.capitalize() #t gets a copied version of s

print(f"s: {s}")
print(f"t: {t}")

