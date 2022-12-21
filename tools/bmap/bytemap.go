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

func ByteDrawCoords (y, x, szc, szl int) {
	wprint(Win, 1, 1, spf("%s%d/%d   %d/%d      ",
		yellow, y+1,szc,   x+1,szl,
	))
	wwrite(Win, spf("%s%d/%d%s    ",
		blue, y*szl+x+1,szc*szl, nc,
	))
}

func (b bytemap) Draw (yoff, xoff int) {
	for i:=0; i<b.height; i++ {
		wmove(Win, i+yoff, xoff)
		for j:=0; j<b.width; j++ {
			if (b.cont[i*b.width+j] == 0) {
				wwrite(Win, RGB(10,10,10)+sEmptyChar)
			} else {
				wwrite(Win, RGB(b.cont[i*b.width+j],b.cont[i*b.width+j],b.cont[i*b.width+j])+sFullChar)
			}
		}
	}
	wwrite(Win, RGB(255,255,255))
}

func ByteDrawPencil (pencil uint8, PutAll int, index int) {
	var prgb = RGB(pencil, pencil, pencil)
	if (pencil < 10) {
		prgb = RGB(10,10,10)
	}
	wprint(Win, 2, 1, spf("pencil:%d: %s%d%s  %s        ", // 9 spaces cuz case "e"
		index, prgb, pencil, nc,
		[]string{"OFF", "SPACE", "ON"}[PutAll+1],
	))
}

func (b bytemap) DByte (yoff, xoff, y, x int) {
	wmove(Win, y+yoff, x+xoff)
	if (b.cont[y*b.width+x] == 0) {
		wwrite(Win, RGB(10,10,10)+sEmptyChar)
	} else {
		wwrite(Win,
			RGB(
				b.cont[y*b.width+x],
				b.cont[y*b.width+x],
				b.cont[y*b.width+x],
			)+sFullChar)
	}
	wwrite(Win, RGB(255,255,255))
}

func (byt bytemap) Interact () {
	var (
		k string
		y, x int
		limy = byt.height; limx = byt.width
		yoff = 3
		xoff = 3
		pencils = make([]uint8, 8)
		pencil uint8 = 255
		penindex = 0
		menu = MakeWin("Save Menu", stdout, stdin, 1, 1+15, 2, 2+50)
		PencilMode = -1 // -1 = off, 0 = space, 1 = on
	)
	for i:=range pencils {
		pencils[i] = 0
	}
	pencils[penindex] = pencil

	Win.name = spf("bytemap editor [%s]", byt.filename)

	// master draw cycle
	wDrawBorderName(Win, '#')
	byt.Draw(yoff, xoff)
	ByteDrawPencil(pencil, PencilMode, penindex)

	for {
		if (PencilMode == 1) { byt.cont[y*byt.width+x] = pencil }
		ByteDrawCoords(y, x, byt.height, byt.width)
		byt.DByte(yoff, xoff, y, x)
		wmove(Win, yoff+y, xoff+x)
		k = wgtk(Win)
		switch (k) {
		case "up", "k":
			y = (y+limy-1)%limy
		case "down", "j":
			y = (y+1)%limy
		case "left", "h":
			x = (x+limx-1)%limx
		case "right", "l":
			x = (x+1)%limx
		case "e": // edit pecil
			wmove(Win, 2, 1)
			wflush(Win)
			pencil = uint8(GetInt("set pencil intencity>"))
			clear()
			wDrawBorderName(Win, '#')
			byt.Draw(yoff, xoff)
			pencils[penindex] = pencil
			ByteDrawPencil(pencil, PencilMode, penindex)
		case "p":
			if (PencilMode == 0) {
				PencilMode = 1
			} else {
				PencilMode = PencilMode*-1
			}
			ByteDrawPencil(pencil, PencilMode, penindex)
		case "P":
			PencilMode = 0
			ByteDrawPencil(pencil, PencilMode, penindex)
		case "c":
			penindex = (penindex+1)%8
			pencil = pencils[penindex]
			ByteDrawPencil(pencil, PencilMode, penindex)
		case "C":
			penindex = (penindex+7)%8
			pencil = pencils[penindex]
			ByteDrawPencil(pencil, PencilMode, penindex)
		case "space":
			if (PencilMode==0) {
				byt.cont[y*byt.width+x] = pencil
			} else {
				byt.cont[y*byt.width+x] = 255-byt.cont[y*byt.width+x]
			}
		case "n":
			clear()
			wprint(Win, 0, 0, spf("current height: %d\n", byt.height))
			wmove(Win, 1, 0)
			wflush(Win)

			addy:=GetInt("y>")

			clear()
			wprint(Win, 0, 0, spf("current width: %d\n", byt.width))
			wmove(Win, 1, 0)
			wflush(Win)

			addx:=GetInt("x>")

			wDrawLine(Win, yoff-1, ' ')
			wDrawBorderName(Win, '#')

			byt.Expand(addy,addx)
			limy = byt.height; limx = byt.width

			clear()
			wDrawBorderName(Win, '#')
			byt.Draw(yoff, xoff)
			ByteDrawPencil(pencil, PencilMode, penindex)
		case "r":
			clear()
			wprint(Win, 0, 0, spf("current name: %s\n", byt.filename))
			wmove(Win, 1, 0)
			wflush(Win)
			name:=oldinput(">")
			byt.filename = name

			Win.name = spf("bytemap editor [%s]", byt.filename)

			wDrawBorderName(Win, '#')
			byt.Draw(yoff, xoff)
			ByteDrawPencil(pencil, PencilMode, penindex)
		case "f9":
			clear()
			wDrawBorderName(Win, '#')
			wDrawBorderName(menu, '#')
			wprint(menu, 2, 2, spf("file: %s", byt.filename))
			wprint(menu, 3, 2, spf("%d bytes to save*", byt.width*byt.height+4))
			// *doesn't account width/height can be bigger than 255
			answ := YorN(menu, "Save/Quit",
				"Save & Quit", "Save",
				"Quit", "Don't save",
			)
			if answ[0] == 'S' {
				byt.save()
			}
			if answ[len(answ)-1] == 't' { return }
			clear()
			wDrawBorderName(Win, '#')
			byt.Draw(yoff, xoff)
			ByteDrawPencil(pencil, PencilMode, penindex)
		}
	}
}

