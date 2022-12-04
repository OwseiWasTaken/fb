package main

include "gutil"
Include "termin"
const (
	FullChar = '@'
	EmptyChar = '.'
	CFullChar = '#'
	CEmptyChar = 'o'
	nc = "\x1b[38;2;255;255;255m"
	yellow = "\x1b[38;2;255;255;0m"
	blue = "\x1b[38;2;0;0;255m"
)

func main(){
	InitTermin()
	bit := ReadBitMap("test.bim")
	bit.filename = "new.bim"
	bit.save()

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

func UnparseBInt(v int) ([]byte) {
	var ret []byte
	var xc byte = 255
	for i:=0;xc != 0;i++{
		xc = uint8(v>>(8*i))
		ret = append([]byte{xc}, ret...)
	}
	return ret[1:]
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

type clr struct {
	R, G, B uint8
}

type bytesmap struct {
	filename string
	cont []clr
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

type bytemap struct {
	filename string
	cont []uint8
	width, height int
}

func ReadBitMap(filename string) (bitmap) {
	var (
		bflcont = ReadFileBytes(filename)
		flcont []int = make([]int, len(bflcont))
		off int = 0
		szh, h int
		szw, w int
		szc int
		c []bool
		bt []byte
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
	w = ParseBInt(szh, flcont[off:])*8
	off += szw

	szc = h*w
	c = make([]bool, szc)
	bt = make([]byte, szc/8)
	//printf("%v\n", c)
	for i:=0;i*8<szc;i++ {
		bt[i] = bflcont[off+i]
		for j:=0;j<8;j++ {
			//printf("%d,%d -> %d\n", i, j, i*8+j)
			c[i*8+j] = bflcont[off+i]&(1<<(7-j%8))!=0
		}
	}
	return bitmap{filename, c, bt, w, h}
}

func (b bitmap) save () {
	var (
		szh = byte(b.height/255+1)
		h = UnparseBInt(b.height)
		szw = byte(b.width/255+1)
		w = UnparseBInt(b.width/8)
		c = b.btcont
		lb = len(b.btcont)
		//c = len(b.cont)
		out = make([]byte, byte(lb)+szh+szw+2)
		off int
		i byte
	)

	out[off] = szh
	off++
	for i=0;i<szh;i++ {
		out[off] = h[i]
		off++
	}

	out[off] = szw
	off++
	for i=0;i<szw;i++ {
		out[off] = w[i]
		off++
	}

	for j:=0; j<lb; j++ {;
		printf("%d -> %d\n", j, lb)
		out[off] = c[j]
		off++
	}

	//for i=0;int(i)<c;i++ {
	//	out[off] = 99
	//	off++
	//}

	PS(b, out)
	err := os.WriteFile(b.filename, out, 0644) // 1X 2W 4R
	panic(err)
}

func (b bitmap) Draw (yoff, xoff int) {
	for i:=0; i<b.height; i++ {
		wmove(Win, i+yoff, xoff)
		for j:=0; j<b.width; j++ {
			if (b.cont[i*b.width+j]) {
				wPut(Win, FullChar)
				//wprint(Win, i+yoff, j+xoff, spf("%c", FullChar))
			} else {
				wPut(Win, EmptyChar)
				//wprint(Win, i+yoff, j+xoff, spf("%c", EmptyChar))
			}
		}
	}
}

func BitDrawCoors (y, x, szc, szl int) {
	wprint(Win, 0, 0, spf("%s%d/%d   %d/%d   ",
		yellow, y+1,szc,   x+1,szl,
	))
	wprint(Win, 1, 0, spf("%s%o/%o   %d/%d   %s",
		blue, y*szl+x,(szc)*(szl-1)+1,   (y*szl+x)/8+1,szc*szl/8, nc,
	))
}

func (bit bitmap) Interact () {
	var (
		k string
		y, x int
		options = []string{"Yes", "No"}
		limy = bit.height; limx = bit.width
		offy = 4
		offx = 3
		menu = MakeWin("Menu", stdout, stdin, 2, 2+15, 2, 2+50)
	)
	Win.name = "bitmap editor"
	wDrawBorderName(Win, '#')
	for {
		DrawCoors(y, x, bit.height, bit.width)
		bit.Draw(offy, offx)
		wmove(Win, offy+y, offx+x)
		k = wgtk(Win)
		switch (k) {
		case "up":
			y = (y+limy-1)%limy
		case "down":
			y = (y+1)%limy
		case "left":
			x = (x+limx-1)%limx
		case "right":
			x = (x+1)%limx
		case "space":
			bit.cont[y*bit.width+x] = !bit.cont[y*bit.width+x]
		case "f9":
			clear()
			wDrawBorderName(Win, '#')
			wDrawBorderName(menu, '#')
			wprint(menu, 2, 2, spf("file: %s", bit.filename))
			wprint(menu, 3, 2, spf("%d bytes to save", bit.width*bit.height/8))
			//TODO: use YorN answ
			YorN(menu, "Save?", options...)
			clear()
			wDrawBorderName(Win, '#')
		}
	}
}

type bitmap struct {
	filename string
	cont []bool
	btcont []byte
	width int
	height int
}

func YorN(win *Window, text string, options ...string) (string) {
	var (
		k string
		x int
		lo = len(options)
		ats = make([]int, lo)
	)
	x = 0
	for i:=0;i<lo;i++ {
		ats[i] = x
		x += len(options[i])+2
	}
	x = 0
	for {
		wmove(win, win.LenY-2, 3)
		for i:=0; i<lo; i++ {
			if i != x {
				wWrite(win, spf("%s  ", options[i]))
			} else {
				wWrite(win, spf("%s%s%s  ", blue, options[i], nc))
			}
		}
		wmove(win, win.LenY-2, 3+ats[x])
		k = wgtk(win)
		for i:=0; i<lo; i++ {
			if k == string(options[0]) {
				return options[i]
			}
		}
		if k == "space" || k == "enter" {
			return options[x]
		}
		if k == "right" || k == "l" {
			x = (x+1)%lo
		}
		if k == "left" || k == "h" {
			x = (x+lo-1)%lo
		}
	}
}

// y/limy   x/limx
// all/szc  byte/szc
func DrawCoors (y, x, szc, szl int) {
	wprint(Win, 1, 1, spf("%s%d/%d   %d/%d   ",
		yellow, y+1,szc,   x+1,szl,
	))
	wprint(Win, 2, 1, spf("%s%d/%d   %d/%d   %s",
		blue, y*szl+x,szc*szl,   (y*szl+x)/8+1,szc*szl/8, nc,
	))
}

func pow(base, power int) (int) {
	for i:=0;i<power;i++ {
		base = base * base
	}
	return base
}
