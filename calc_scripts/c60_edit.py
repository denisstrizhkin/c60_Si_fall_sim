import sys

AXESDICT ={'x':2, 'y':3, 'z':4}
FILEPATH = sys.argv[1]
FIRSTID = int(sys.argv[2])
LASTID = int(sys.argv[3])
AXIS = sys.argv[4]
COORD = AXESDICT[AXIS]
DELTA = float(sys.argv[5])

def main():
	with open(FILEPATH, 'r') as input_file:
		lines = input_file.readlines()

	pos = 0
	while pos < len(lines):
		if lines[pos][0:5] == 'Atoms':
			break
		pos += 1
	pos += 2

	while len(lines[pos]) > 2:
		cur_str = lines[pos].split(' ')
		
		if int(cur_str[0]) >= FIRSTID and int(cur_str[0]) <= LASTID:
			cur_str[COORD] = str(float(cur_str[COORD]) + DELTA)
			lines[pos] = cur_str[0]
			
			for i in range(1, len(cur_str)):
				lines[pos] += ' ' + cur_str[i]
		pos += 1

	print('fu moved in {0}-axis by ({1})'.format(AXIS, DELTA))

	with open(FILEPATH, 'w') as output_file:
		for line in lines:
			print(line, end='', file=output_file)

if __name__ == '__main__':
	main()
