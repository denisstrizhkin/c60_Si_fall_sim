import sys
import numpy as np

# global vars
FILE_PATH = sys.argv[1]
ZERO_LEVEL = float(sys.argv[2])
NUM_ATOMS = 60

def main():
    # read dump file
    with open(FILE_PATH, 'r') as input_file:
    	lines = input_file.readlines()
        
    head = lines[8].split(' ')
    head[len(head) - 1] = head[len(head) - 1][:-1]
    keys = dict()

    for i in range (2, len(head)):
        keys[head[i]] = i
   
    timesteps = dict()
    for i in range (0, len(lines)):
        if lines[i] == "ITEM: TIMESTEP\n":
            print(lines[i + 1])
            print(lines[i + 3])

    return None

if __name__ == '__main__':
	main()
