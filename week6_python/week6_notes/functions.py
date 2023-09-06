#creating a function

def main(): #only used for convention
    for i in range(3):
        meow()

def meow():
    print("meow")

main()

#now parameterize

def main():
    meow(3)

def meow(n):
    for i in range(n):
        print("meow")

main()