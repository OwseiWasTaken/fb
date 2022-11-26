#! /usr/local/bin/python3.10
#imports
from util import *

def canpack(cont: str) -> bool:
	return len(set(i for i in cont)) == 2

def pack(cont: str) -> list[int]:
	bins = list(map(lambda x: "0" if x in " .·-" else "1", cont))
	ints = []
	for i in r(16):
		ints.append(int( ''.join(bins[i*8:(i+1)*8]) , 2))
	return ints

def unpackfile(ints: list[int]) -> list[str]:
	bins = []
	for i in r(len(ints)//16):
		bins.append(unpack(ints[i*16:(i+1)*16]))
	return bins
def unpack(cont: list[int]) -> str:
	cont = list(map(int, cont))
	ret = ""
	for i in r(cont):
		ret+='|'+bin(cont[i])[2:].replace('1', '@').replace('0', ' ')+"|\n"
	return ret[:-1]

#def PrintChars

def main() -> int:
	if get("--read").exists:
		file = "draw/font"
		if x:=get("--read").first:
			file = x
		with open(file, "rb") as f:
			chars = unpackfile(f.readlines()[0])
		for i in r(chars):
			print(f"\b\nChar: {i}")
			print(''.join(chars[i]))
		return 0

	image = "./draw/fontimage"
	editor = getenv("EDITOR")
	if not editor:
		print("$EDITOR isn't defined\n")
		return 1

	if (x:=cmd(f"{editor} {image}")).returncode:
		print("editor broke\n")
		return x.returncode

	with open(image) as f:
		file = ''.join(f.readlines()).replace("\n", "")
		if not len(file):return 3

	if canpack(file):
		packed = pack(file)
		print(unpackfile(packed))
		print(packed)
	else:
		print("can't pack /tmp/tofont's content")
		return 2

	return 0



#start
if __name__ == '__main__':
	start = tm()
	ExitCode = main()

	if get('--timeit').exists:
		if not ExitCode:
			print("%scode successfully exited in " % RGB(0,255,0))
		else:
			print("%scode exited with error %d in " % (RGB(255,0,0),ExitCode))
		print("%.3f seconds%s" % (tm()-start,RGB(255,255,255)))
	exit(ExitCode)
