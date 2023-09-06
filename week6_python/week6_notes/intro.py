from cs50 import get_string

#printing
answer = get_string("What's your name?")
print(f"hello, {answer}")
print("hello, world")

#conditionals
if x < y:
    print("x is less than y")
else:
    print("x is not less than y")

if x < y:
    print("x is less than y")
elif x > y:
    print("x is greater than y")
else:
    print("x is equal to y")

#variables
counter = 0
counter = counter + 1
counter += 1

#loops
i = 0
while i < 3:
    print("meow")
    i += 1

for i in range(3):
    print("hello, world")

#forever blocks
while True:
    print("meow")
