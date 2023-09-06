s = input("Do you agreee? ")

if s == "Y" or s == "y":
    print("Agreed.")
elif s == "N" or s == "n":
    print("Not agreed.")

#another way
if s in ["Y", "y"]:
    print("Agreed.")
elif s in ["N", "n"]:
    print("Not agreed.")

#accounts for more variation. Uses "methods", or functions within variables
#accessing a function inside of a value rather than passing a value through a function
s = s.lower()
if s in ["y", "yes"]:
    print("Agreed.")
elif s in ["n", "no"]:
    print("Not agreed.")