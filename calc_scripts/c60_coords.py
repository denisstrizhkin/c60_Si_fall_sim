import sys

FILE_PATH = './fall.output.data'
START_INDEX = 16
AXES = {'x':2, 'y':3, 'z':4}
INIT_ID = 162432
ZERO_LVL = -0.0184635

def main():
	with open(FILE_PATH, 'r') as input_file:
		lines = input_file.readlines()

	pos = START_INDEX
	coords = list()
	
	while len(lines[pos]) > 2:
		cur_line = lines[pos].split(' ')
		
		if int(cur_line[0]) > INIT_ID:
			coords.append(float(cur_line[AXES[sys.argv[1]]]))
		
		pos += 1
	
	level = 0.0	

	if sys.argv[1] == 'z':
		level = ZERO_LVL
	else:
		for item in coords:
			level += item
		level /= 60

	for i in range(0, len(coords)):
		coords[i] -= level
	
	for item in coords:
		print(item)

	print("{0} atoms".format(len(coords)))

	return None

if __name__ == '__main__':
	main()
