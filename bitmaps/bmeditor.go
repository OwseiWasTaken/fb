package main

include "gutil"
Include "termin"

func main(){
	InitTermin()
	x:=ReadBytesMap("test.btsm")
	PS(x)
	y:=ReadByteMap("test.btm")
	PS(y)

	StopTermin()
	exit(0)
}

// n, offset
func ParseBInt(len int, bts []int) (int) {
	var x int = 0
	var i int
	for i = 0; i < len; i++ {
		x+= 255*x+bts[i]
	}
	return x
}

func ReadBytesMap(filename string) (bytesmap) {
	var (
		bflcont = ReadFileBytes(filename)
		flcont []int = make([]int, len(bflcont))
		off int = 0
		szh int
		h int
		szw int
		w int
		szc int
		c []int
		cs []clr
	)
	for i:=0;i<len(bflcont);i++ {
		flcont[i] = int(bflcont[i])
	}
	szh = flcont[off]
	off++
	h = ParseBInt(szh, flcont[off:])
	off += szh

	//printf("@%d in %v\n", off, flcont)
	szw = flcont[off]
	off++
	w = ParseBInt(szh, flcont[off:])
	off += szw

	szc = 3*h*w
	c = flcont[off:off+szc]
	//printf("%v\n", c)
	cs = make([]clr, len(c)/3)
	for i:=0;i<len(c);i+=3 {
		//printf("@%d in %v\n»@%d in %v\n", i, c, i, cs)
		cs[i/3] = clr{
			uint8(c[i+0]),
			uint8(c[i+1]),
			uint8(c[i+2]),
		}
	}
	return bytesmap{filename, cs, w, h}
}

type clr struct {
	R uint8
	G uint8
	B uint8
}

type bytesmap struct {
	filename string
	cont []clr
	width int
	height int
}

func ReadByteMap(filename string) (bytemap) {;
	var (
		bflcont = ReadFileBytes(filename)
		flcont []int = make([]int, len(bflcont))
		off int = 0
		szh int
		h int
		szw int
		w int
		szc int
		c []uint8
	)
	for i:=0;i<len(bflcont);i++ {
		flcont[i] = int(bflcont[i])
	}
	szh = flcont[off]
	off++
	h = ParseBInt(szh, flcont[off:])
	off += szh

	//printf("@%d in %v\n", off, flcont)
	szw = flcont[off]
	off++
	w = ParseBInt(szh, flcont[off:])
	off += szw

	szc = w*h
	c = make([]uint8, szc)
	//printf("%v\n", c)
	for i:=0;i<szc;i++ {
		c[i] = bflcont[off+i]
	}
	return bytemap{filename, c, w, h}
}

type bytemap struct {
	filename string
	cont []uint8
	width int
	height int
}

func ReadBitMap(filename string) (bitmap) {
	var (
		bflcont = ReadFileBytes(filename)
		flcont = []int{}
		off int
		szh int
		h int
		szw int
		w int
		szc int
		c []int
	)
	for i:=0;i<len(bflcont);i++ {
		flcont[i] = int(bflcont[i])
	}
	return bitmap{}
	szh = flcont[off]
	off++
	h = ParseBInt(szh, flcont[off:])
	off += szh

	//printf("@%d in %v\n", off, flcont)
	szw = flcont[off]
	off++
	w = ParseBInt(szh, flcont[off:])*8
	off += szw

	szc = h*w
	c = make([]bool, szc)
	//printf("%v\n", c)
	for i:=0;i<szc;i++ {
		for j:=0;j<8;j++ {
			c[i*8+j] = bflcont[off+i]&(1<<(8-j%8))
		}
	}
	return bitmap{filename, c, w, h}
}

type bitmap struct {
	filename string
	cont []bool
	width int
	height int
}

