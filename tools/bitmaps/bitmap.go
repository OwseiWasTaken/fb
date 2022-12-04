type bitmap struct {
	filename string
	cont []bool
	btcont []byte
	width int
	height int
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
