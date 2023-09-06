from sys import argv

if len(argv) == 2:
    print(f"hello, {argrv[1]}")
else:
    print("hello, world")

#or

for i in range(len(argv)):
    print(argv[1])

#even simpler

for arg in argv: #or any variable name
    print(arg)

#without argv[0]
for arg in argv[1:]: #starts at location one and goes all the way to the end of the list (slicing)
    print(arg)



