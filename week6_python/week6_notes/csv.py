import csv

file = open("phonebook.csv", "a")

name = input("Name: ")
number = input("Number: ")

writer = csv.writer(file)
writer.writerow([name,number])

file.close()

#better way to open to avoid closing:

with open("phonebook.csv", "a") as file: #closes file at the end of indentation

    name = input("Name: ")
    number = input("Number: ")

    writer = csv.writer(file)
    writer.writerow([name,number])

#using Dict Writer

with open("phonebook.csv", "a") as file:

    name = input("Name: ")
    number = input("Number: ")

    writer = csv.DictWriter(file, fieldnames=["name", "number"])
    writer.writerow({"name": name, "number": number})
