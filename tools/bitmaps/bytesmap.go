type clr struct {
	R, G, B uint8
}

type bytesmap struct {
	filename string
	cont []clr
	width, height int
}

func ReadBytesMap(filename string) (bytesmap) {
	var (
		bflcont = ReadFileBytes(filename)
		flcont []int = make([]int, len(bflcont))
		off int = 0
		szh, h int
		szw, w int
		szc int
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
	cs = make([]clr, szc/3)
	for i:=0;i<szc;i+=3 {
		cs[i/3] = clr{
			uint8(flcont[off+i+0]),
			uint8(flcont[off+i+1]),
			uint8(flcont[off+i+2]),
		}
	}
	return bytesmap{filename, cs, w, h}
}
