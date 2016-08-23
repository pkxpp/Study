import os
import sys

def recursive(dir_name, tag):
	names = os.listdir(dir_name)
	for name in names:
		# relative path ---> abolute path
		name_temp = dir_name + "\\" + name
		#print(name_temp)
		if os.path.isdir(name_temp): 
			if tag is True:
				print(name_temp)
				recursive(name_temp, tag)
			else:
				continue
		if os.path.isfile(name_temp):
			print(name_temp)

def main(dir_name, tag):
	recursive(dir_name, tag)

if __name__ == '__main__':
	path = os.getcwd()
	tag = False;
	if ( len(sys.argv) >= 2 ):
		path = sys.argv[1]
	
	if ( len(sys.argv) >= 3 ):
		tag = sys.argv[2]
		
	main(path, tag)
