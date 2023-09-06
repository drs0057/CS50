from cs50 import get_int
scores = [72, 73, 33]

average = sum(scores) / len(scores)
print(f"Average: {average}")

#from user; appending to a list
scores = []
for i in range(3):
    score = get_int("Score: ")
    scores.append(score) # or: scores += [score]

average = sum(scores) / len(scores)
print(f"Average: {average}")
