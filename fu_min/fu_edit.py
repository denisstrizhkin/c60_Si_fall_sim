import sys

FILEPATH='fu_min.input.data'
AXES={'x':2, 'y':3, 'z':4}

def id_add(lines, pos):
	while pos < len(lines):
		cur_str = lines[pos].split(' ', 1)
		lines[pos] = str(int(cur_str[0]) + int(sys.argv[2])) + ' ' + cur_str[1]
		pos += 1

	print('added ({0}) to all ids'.format(sys.argv[2]))
	return None

def move(lines, pos):
	while pos < len(lines):
		cur_str = lines[pos].split(' ')
		cur_str[AXES[sys.argv[2]]] = str(float(cur_str[AXES[sys.argv[2]]]) + float(sys.argv[3]))
		
		lines[pos] = cur_str[0]
		for i in range(1, len(cur_str)):
			lines[pos] += ' ' + cur_str[i]
		lines[pos] += '\n'
		pos += 1

	print('fu moved in {0}-axis by ({1})'.format(sys.argv[2], sys.argv[3]))
	return None

def main():
	with open(FILEPATH, 'r') as input_file:
		lines = input_file.readlines()

	pos = 0
	while pos < len(lines):
		if lines[pos][0:5] == 'Atoms':
			break
		pos += 1
	pos += 2

	if sys.argv[1] == 'id_add':
		id_add(lines, pos)
	elif sys.argv[1] == 'move':
		move(lines, pos)

	with open(FILEPATH, 'w') as input_file:
		for line in lines:
			print(line, end='', file=input_file)

if __name__ == '__main__':
	main()
