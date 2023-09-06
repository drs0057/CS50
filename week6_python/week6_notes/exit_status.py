import sys

if len(sys.argv) != 2: #refers to "argv" within library "sys"
    print("Missing command-line argument")
    sys.exit(1)

print(f"hello, {sys.argv[1]}")
sys.exit(0)

#remember use echo $? to see exit status