import sys
import numpy as np

# global vars
FILE_PATH = sys.argv[1]
ZERO_LEVEL = float(sys.argv[2])
NUM_ATOMS = 60
AXES = {'x':2, 'y':3, 'z':4}

def main():
    # read dump file
    with open(FILE_PATH, 'r') as input_file:
    	lines = input_file.readlines()
    
    num_of_lines = len(lines)
    num_of_atoms = int(lines[3])
    
    head = lines[8].split(' ')
    head[len(head) - 1] = head[len(head) - 1][:-2]
    keys = dict()

    for i in range (2, len(head)):
        keys[head[i]] = i
   
    print(keys)

    return None

if __name__ == '__main__':
	main()
