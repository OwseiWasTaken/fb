#! /usr/local/bin/python3.10
#imports
from util import *

# code from nBDP.WriteInt from util.py
def itob(cont:int):
	x = 0
	c = cont
	while c:
		x+=1
		c = c>>1
	if d:=x%8:
		d = 8-d
		x+=d
	# // to easly keep int
	x=x//8
	# set bytes
	# invert
	rs = ('0'*d) + bin(cont)[2:]
	ret = []
	for i in r(x):
		i*=8
		z = rs[i:i+8]
		ret.append(int(z, 2))
	return x, ret

def ParseIntList(ints:list[int]):
	# get size (from type)
	size = len(ints)
	x = 0
	for i in r(size):
		# +=*9 = =*10
		x+=x*255+ints[i]
	return x

# 1b size of h
# #b h
# 1b size of w
# #b w
# 1b size of size of cont
# #b size of cont
#	3#b content
class ByteMap:
	def __init__(this, h:int, w:int, data:list[tuple[int, int, int]], filename=None):
		this.h = h
		this.w = w
		this.data = data
		this.filename = filename

	def save(this, filename=None):
		file = this.filename
		if filename:
			file = filename
		assert file != None, "need filename"
		with open(file, 'wb') as f:
			f.write(bytes( SingleList(list(map( itob, [this.h, this.w]) )) ))
			f.write(bytes( SingleList(itob(len(SingleList(this.data))//3)) ))
			f.write(bytes( SingleList(this.data) ))

	def show(this):
		print(f"{this.h}X{this.w}")
		for i in r(this.data):
			R,G,B = this.data[i]
			print("@", end=RGB(R, G, B))
			if not (i+1)%this.w:
				print()

	def __repr__(this):
		return f"{this.h}X{this.w}\n{this.data}"

def ReadInt(f, size):
	return int.from_bytes(f.read(size), "big")

def ReadByteMap(filename:str) -> ByteMap:
	h=0
	w=0
	cont=0
	with open(filename, "rb") as f:
		SzOfH = ReadInt(f, 1)
		h = ReadInt(f, SzOfH)

		SzOfW = ReadInt(f, 1)
		w = ReadInt(f, SzOfW)

		SzOfSzOfCont = ReadInt(f, 1)
		SzOfCont = ReadInt(f, SzOfSzOfCont)
		cont = [(ReadInt(f, 1), ReadInt(f, 1), ReadInt(f, 1)) for i in r(SzOfCont)]

	return ByteMap(h, w, cont)

def main() -> int:
	read = get("-r", "--read")
	doread = read.exists
	readfile = read.first
	assert (not readfile) ^ doread, "if --read, file must be provided"

	write = get("-w", "--write")
	dowrite = write.exists
	writefile = write.first
	assert (not writefile)^ dowrite, "if --write, file must be provided"

	b = ByteMap(2,2, [
		(255,255,255),(  0,  0,255),
		(255,  0,  0),(  0,255,  0),
	], "test.btm")
	b.save()

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
