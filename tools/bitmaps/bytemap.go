type bytemap struct {
	filename string
	height, width int
	cont []uint8
}

func MakeByteMap(filename string, h, w int) (bytemap) {
	return bytemap{filename+".btm", h, w, make([]uint8, w*h)}
}

//////////////////////////
////////// back //////////
//////////////////////////

func ReadByteMap(filename string) (bytemap) {
	ret := MakeByteMap(filename, 0, 0)
	var (
		bflcont = ReadFileBytes(ret.filename)
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
	ret.width = w
	ret.height = h
	ret.cont = c
	return ret
}

func (b bytemap) save () {
	var (
		szh = b.height/255+1
		h = UnparseBInt(b.height)
		szw = b.width/255+1
		w = UnparseBInt(b.width)
		out = make([]byte, len(b.cont)+szh+szw+2) // +2 -> write szh, szw
		off int
		i int
	)
	out[off] = byte(szh)
	off++
	for i=0;i<szh;i++ {
		out[off] = h[i]
		off++
	}

	out[off] = byte(szw)
	off++
	for i=0;i<szw;i++ {
		out[off] = w[i]
		off++
	}

	for i=0; i<len(b.cont); i++ {;
		//printf("%d -> %d\n", j, lb)
		out[off] = b.cont[i]
		off++
	}

	err := os.WriteFile(b.filename, out, 0644) // 1X 2W 4R
	panic(err)
}

func (b bytemap) copy (filename string) {
	k := b.filename
	b.filename = filename+".btm"
	b.save()
	b.filename = k
}

func (b *bytemap) Expand (y, x int) {
	if (y < 0 || x < 0) {die("can't expand bytemap by negative numbers")}
	var nh = b.height+y
	var nw = b.width+x
	var ncont = make([]uint8, nh*nw)
	for i:=0; i<nh; i++ {
		for j:=0; j<nw; j++ {
			if (b.width > j) && (b.height > i) {
				ncont[nw*i+j] = b.cont[b.width*i+j]
			}
		}
	}
	b.width = nw
	b.height = nh
	b.cont = ncont
}

//////////////////////////
////////// front /////////
//////////////////////////

func ByteDrawCoors (y, x, szc, szl int) {
	wprint(Win, 0, 0, spf("%s%d/%d   %d/%d   ",
		yellow, y+1,szc,   x+1,szl,
	))
	wprint(Win, 1, 0, spf("%s%o/%o   %d/%d   %s",
		blue, y*szl+x,(szc)*(szl-1)+1,   (y*szl+x),szc*szl, nc,
	))
}

func (b bytemap) Draw (yoff, xoff int) {
	for i:=0; i<b.height; i++ {
		wmove(Win, i+yoff, xoff)
		for j:=0; j<b.width; j++ {
			wwrite(Win, RGB(b.cont[i*b.width+j],b.cont[i*b.width+j],b.cont[i*b.width+j])+sFullChar)
		}
	}
}

func (b bytemap) Interact () { }

