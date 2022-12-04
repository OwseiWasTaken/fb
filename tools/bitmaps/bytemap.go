type bytemap struct {
	filename string
	cont []uint8
	width, height int
}

func ReadByteMap(filename string) (bytemap) {;
	var (
		bflcont = ReadFileBytes(filename)
		flcont []int = make([]int, len(bflcont))
		off int = 0
		szh, h int
		szw, w int
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
