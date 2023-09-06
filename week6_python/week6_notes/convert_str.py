before = input("Before: ")
print("After: ", end="")
for c in before:
    print(c.upper(), end="")
print()

#simpler way
before = input("Before: ")
after = before.upper()
print(f"After: {after}")
