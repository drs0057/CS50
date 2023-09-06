while True: #prompts user until they cooperate
    n = int(input("Height: "))
    if n > 0:
        break

for i in range(n):
    print("#")

#using functions. Using the try function to handle user inputting a string

def main():
    height = get_height()
    for i in range(height):
        print("#")

def get_height():
    while True:
        try:
            n = int(input("Height: "))
            if n > 0:
                return n
        except ValueError:
            print("Not an integer.")


