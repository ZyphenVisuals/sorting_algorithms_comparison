import sys
import random
import os

# check if length exists
if len(sys.argv) != 2:
    print("Command usage: python dataset_generator.py [number]")
    sys.exit()

# get length
length = sys.argv[1]
print(f"Generating datasets with length {length}")

# check if files exist
if os.path.isfile(f"./data/{length}_sequential.txt"):
    print(f"Datasets for {length} already exist. Do you want to replace them? (y/N)")
    choice = input()
    if not (choice == "y" or choice == "Y"):
        sys.exit()


# write sequential dataset
with open(f"./data/{length}_sequential.txt", "w") as file:
    lines = []
    for i in range(int(length)):
        lines.append(str(i) + "\n")
    file.writelines(lines)

# write random dataset
with open(f"./data/{length}_random.txt", "w") as file:
    lines = []
    for i in range(int(length)):
        lines.append(str(random.randint(0, 2147483647)) + "\n")
    file.writelines(lines) 

# write reverse sequential dataset
with open(f"./data/{length}_inverse_sequential.txt", "w") as file:
    lines = []
    for i in range(int(length)):
        lines.append(str(int(length)-i) + "\n")
    file.writelines(lines)

print("Files written.")