#overrides the end of each line to be "" rather than "\n"
for i in range(4):
    print("?", end="") #"named" arguement rather than "positional"
print() #makes new line

#or
print("?" * 4) #concatenacion again and again

#now for a grid
for i in range(3):
    print("#" * 3)