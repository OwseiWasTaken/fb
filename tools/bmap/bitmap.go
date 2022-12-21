type bitmap struct {
	filename string
	cont []bool
	btcont []byte
	height int
	width int
}

func MakeBitMap(filename string, h, w int) (bitmap) {
	return bitmap{filename+"bim", make([]bool, h*w), make([]byte, h*w/8), h, w}
}

//////////////////////////
////////// back //////////
//////////////////////////

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
	w = ParseBInt(szh, flcont[off:])
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
	ret := MakeBitMap(filename, h, w)
	ret.cont = c
	ret.btcont = bt
	return ret
}

func (b bitmap) save () {
	var (
		szh = b.height/255+1
		h = UnparseBInt(b.height)
		szw = b.width/255+1
		w = UnparseBInt(b.width/8)
		// TODO: compact bool cont -> byte cont to save
		c = b.btcont
		lb = len(b.btcont)
		//c = len(b.cont)
		out = make([]byte, lb+int(szh+szw)+2)
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

	for i:=0; i<lb; i++ {;
		//printf("%d -> %d\n", j, lb)
		out[off] = c[i]
		off++
	}

	//for i=0;int(i)<c;i++ {
	//	out[off] = 99
	//	off++
	//}

	err := os.WriteFile(b.filename, out, 0644) // 1X 2W 4R
	panic(err)
}

func (b bitmap) copy (filename string) {
	k := b.filename
	b.filename = filename
	b.save()
	b.filename = k
}

func (b bitmap) export (tp int, filename string) {
	fo, err := os.Create(filename)
	panic(err)
	defer func() {
		panic(fo.Close())
	}()
	switch tp {
		case Exp_C:
			fprintf(fo, "bitmap b;\n")
			fprintf(fo, "b.height = %d;\n", b.height)
			fprintf(fo, "b.width = %d;\n", b.width)
			fprintf(fo, "uint8 cont[] = {\n\t")
			i:=0
			for ; i<(b.width*b.height/8)-1; i++ {
				if i%(b.width/8) == 0 && i > 0{
					fprintf(fo, "\n\t")
				}
				fprintf(fo, "%3d,", int(b.btcont[i]))
			}
			fprintf(fo, "%d\n", int(b.btcont[i]))
			fprintf(fo, "};\n")
			fprintf(fo, "b.cont = cont;\n")
		default:
			printf("No such Export type %d\n", tp)
	}
}

func (b *bitmap) Expand (y, x int) {
	if (y < 0 || x < 0) {die("can't expand bitmap by negative numbers")}
	var nh = b.height+y
	var nw = b.width+x
	var ncont = make([]bool, (nh)*(nw))
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

///////////////////////////
////////// front //////////
///////////////////////////

func (b bitmap) Draw (yoff, xoff int) {
	for i:=0; i<b.height; i++ {
		wmove(Win, i+yoff, xoff)
		for j:=0; j<b.width; j++ {
			if (b.cont[i*b.width+j]) {
				wput(Win, FullChar)
				//wprint(Win, i+yoff, j+xoff, spf("%c", FullChar))
			} else {
				wput(Win, EmptyChar)
				//wprint(Win, i+yoff, j+xoff, spf("%c", EmptyChar))
			}
		}
	}
}

func BitDrawCoords (y, x, szc, szl int) {
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
		limy = bit.height; limx = bit.width
		offy = 4
		offx = 3
		menu = MakeWin("Menu", stdout, stdin, 2, 2+15, 2, 2+50)
		DelAll = false
		PutAll = false
	)
	Win.name = spf("bitmap editor [%s]", bit.filename)
	wDrawBorderName(Win, '#')
	for {
		if DelAll {
			bit.cont[y*bit.width+x] = false
		} else if PutAll {
			bit.cont[y*bit.width+x] = true
		}

		DrawCoords(y, x, bit.height, bit.width)
		bit.Draw(offy, offx)
		wmove(Win, offy+y, offx+x)
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
		case "space":
			bit.cont[y*bit.width+x] = !bit.cont[y*bit.width+x]
		case "c":
			DelAll = false
			PutAll = !PutAll
		case "x":
			DelAll = !DelAll
			PutAll = false
		case "z":
			DelAll = false
			PutAll = false

		case "^L":
			for i:=0; i<bit.height; i++ {
				for j:=0; j<bit.width; j++ {
					bit.cont[bit.width*i+j] = false
				}
			}
		case "^K":
			for i:=0; i<bit.height; i++ {
				for j:=0; j<bit.width; j++ {
					bit.cont[bit.width*i+j] = true
				}
			}

		case "n":
			clear()
			wprint(Win, 0, 0, spf("current height: %d\n", bit.height))
			wmove(Win, 1, 0)
			wflush(Win)

			addy:=GetInt("y>")

			clear()
			wprint(Win, 0, 0, spf("current width: %d\n", bit.width))
			wmove(Win, 1, 0)
			wflush(Win)

			addx:=GetInt("x>")

			wDrawLine(Win, offy-1, ' ')
			wDrawBorderName(Win, '#')

			bit.Expand(addy,addx)
			limy = bit.height; limx = bit.width

		case "r":
			clear()
			wprint(Win, 0, 0, spf("current name: %s\n", bit.filename))
			wmove(Win, 1, 0)
			wflush(Win)
			name:=oldinput(">")
			bit.filename = name

			wDrawBorderName(Win, '#')
			Win.name = spf("bitmap editor [%s]", bit.filename)

		case "f9":
			clear()
			wDrawBorderName(Win, '#')
			wDrawBorderName(menu, '#')
			wprint(menu, 2, 2, spf("file: %s", bit.filename))
			wprint(menu, 3, 2, spf("%d bytes to save", bit.width*bit.height/8))
			answ := YorN(menu, "Save?", "Yes", "No")
			if answ == "Yes" {
				bit.save()
				return
			}
			clear()
			wDrawBorderName(Win, '#')
		}
	}
}
