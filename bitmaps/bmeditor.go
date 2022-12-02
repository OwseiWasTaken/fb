package main

include "gutil"
Include "termin"

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
		szszc int
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

	//printf("@%d in %v\n", off, flcont)
	szszc = flcont[off]
	off++
	szc = 3*ParseBInt(szszc, flcont[off:])
	off += szszc
	//printf("@%d in %v\n", off, flcont)
	c = flcont[off:off+szc]
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

func main(){
	InitTermin()
	x:=ReadBytesMap("test.btsm")
	PS(x)

	StopTermin()
	exit(0)
}




type bytemap struct {
	filename string
	cont []uint8
	width int
	height int
}

type bitmap struct {
	filename string
	cont []bool
	width int
	height int
}

