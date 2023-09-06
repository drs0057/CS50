# people = dict()
# or people = {}

people = {
    "Carter": "+1-617-495-1000",
    "David": "+1-949-468-2750"
}

name = input("Name: ")
if name in people:
    print(f"Number: {people[name]}") #can index in using a name, not a number

