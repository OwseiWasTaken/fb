package vars

// id
const (
	// offset for export {self, C}
	MAP_ = iota
	MAP_C
	MAP_bit
	MAP_byte
	MAP_bytes
)

var (
	TextToId = map[string]int{
		"":MAP_,
		"bit":MAP_bit,
		"bitm":MAP_bit,
		"mbit":MAP_bit,
		"bitmap":MAP_bit,
		"mapbit":MAP_bit,
		"bmap":MAP_bit,
		"bimap":MAP_bit,
		"bim":MAP_bit,
		"bi":MAP_bit,

		"byte":MAP_byte,
		"bytem":MAP_byte,
		"mbyte":MAP_byte,
		"bytemap":MAP_byte,
		"mapbyte":MAP_byte,
		"bymap":MAP_byte,
		"bytmap":MAP_byte,
		"btmap":MAP_byte,
		"btm":MAP_byte,
		"bt":MAP_byte, // bruh

		"bite":MAP_byte,
		"bitem":MAP_byte,
		"mbite":MAP_byte,
		"bitemap":MAP_byte,
		"mapbite":MAP_byte,

		"bytes":MAP_bytes,
		"bytesm":MAP_bytes,
		"mbytes":MAP_bytes,
		"bytesmap":MAP_bytes,
		"mapbytes":MAP_bytes,
		"bytsmap":MAP_bytes,
		"bysmap":MAP_bytes,
		"btsmap":MAP_bytes,
		"btsm":MAP_bytes,
		"bts":MAP_bytes,


		"bites":MAP_bytes,
		"bitesm":MAP_bytes,
		"mbites":MAP_bytes,
		"bitesmap":MAP_bytes,
		"mapbites":MAP_bytes,

		"on/off":MAP_bit,
		"on,off":MAP_bit,
		"on off":MAP_bit,
		"on or off":MAP_bit,
		"cheap":MAP_bit,
		"white":MAP_bit,
		"black":MAP_bit,
		"black and white":MAP_bit,
		"black white":MAP_bit,

		"intensity":MAP_byte,
		"intense":MAP_byte,
		"intst":MAP_byte,
		"itst":MAP_byte,

		"color":MAP_bytes,
		"colors":MAP_bytes,
		"full":MAP_bytes,
		"colorful":MAP_bytes,
		"colorfull":MAP_bytes,
	}
)

const (
	HERE = "\x1b[38;2;255;0;0mHERE\x1b[38;2;255;255;255m"

	// io.go
	FILEINTSIZE = 4
	SIZEOFBYTE = 8

	// editor.go
	EDITOR_YOFF = 3
	EDITOR_XOFF = 10

	// YorN
	YORN_YOFF = 1
	YORN_XOFF = 2
	COL_HL = "\x1b[38;2;255;128;0m"
	COL_NC = "\x1b[38;2;255;255;255m"

	// MakeMap()
	DEFAULT_MAP_Y = 10
	DEFAULT_MAP_X = 10
)

// cut down on interface funcs
// make map specific .interacts
type FlMap interface {
	GetId() (int) // get type
	GetObj() (string) // get name wo/ etx
	GetSize() (int,int)
	GetByteArr() ([]byte)
	// self -> bytesmap
	GetPxArray() ([]pixel)
	// bytesmap -> self
	SetPxArray([]pixel) ()
}

type point struct { y, x int }

var (
	MapToExt = []string{
		"", // self
		".c", // C code
		".bim", // BIt Map
		".btm", // ByTe Map
		".btsm", // ByTeS Map
	}
	MapToName = []string{
		"self", // self
		"c", // C code
		"bitmap", // BIt Map
		"bytemap", // ByTe Map
		"bytesmap", // ByTeS Map
	}
)

type pixel struct { R,G,B byte }

// bytes -> int
func ParseBInt(bts []byte) (int) {
	b := 0
	for i:=0;i<FILEINTSIZE;i++ {
		b += int(bts[i]<<(SIZEOFBYTE*(FILEINTSIZE-i-1)))
	}
	return b
}

// int -> bytes
func UnparseBInt(v int) ([]byte) {
	b := make([]byte, FILEINTSIZE)
	for i:=0;i<FILEINTSIZE;i++ {
		b[i] = byte(v>>(SIZEOFBYTE*FILEINTSIZE-i-1))
	}
	return b
}

func ExpandBytes(v []byte) (ret []bool) {
	b := make([]bool, len(v)*SIZEOFBYTE)
	for i := 0; i<len(v); i++ {
		e := ExpandByte(v[i])
		for j := 0; j < SIZEOFBYTE; j++ {
			b[i*SIZEOFBYTE+j] = e[j]
		}
	}
	return b
}

func ExpandByte(v byte) (ret []bool) {
	b := make([]bool, SIZEOFBYTE)
	for i := 0; i < SIZEOFBYTE; i++ {
		b[i] = v&(1<<(SIZEOFBYTE-i-1))!=0
	}
	return b
}

func CompressByte(b []bool) (ret byte) {
	ret = 0
	for i := range b {
		ret += bog(b[i], byte(1<<(len(b)-i-1)), byte(0)).(byte)
		//ret += bog(b[i], byte(1<<i), byte(0)).(byte)
	}
	return ret
}

func _GetType(name string) (int) {
	for i := range MapToExt {
		if (MapToExt[i] == name) {return i}
	}
	assert(false, spf("_GetType: can't find ext %s", name))
	return -1
}

func GetObjAndType(name string) (obj string, id int) {
	dot := strings.Index(name, ".")
	//assert(dot > 0, "GetObjAndType: no '.' in filename")
	if dot < 0 {return name, MAP_}
	id = _GetType(name[dot:])
	obj = name[:dot]
	return
}

func MakeMap(filename string, id, w, h int) (FlMap) {
	var b FlMap = nil
	switch (id) {
	case MAP_bit:
		b = MakeBitmap(filename, w, h)
	case MAP_byte:
		b = MakeBytemap(filename, w, h)
	case MAP_bytes:
		b = MakeBytesmap(filename, w, h)
	default:
		die(spf("MakeMap can't create id %d", id))
	}
	return b
}

func pxMean(p pixel) (byte) {
	return byte((int(p.R)+int(p.G)+int(p.B))/3)
}

func YorN (szy, szx int, name string, options ...string) (int) {
	// find linefeeds
	ats := make([]point, len(options))
	yat := YORN_YOFF // tmp
	{
		xat := YORN_XOFF // tmp

		for i := range options {
			if options[i] == "\n" {
				ats[i] = point{0, 0}
				yat++
				xat = YORN_XOFF
			} else {
				ats[i] = point{yat, xat}
				xat+=len(options[i])+1
			}
		}
	}
	if szy < 0 {
		szy = szy*-1 + yat-YORN_YOFF + 2
	}

	// biggest option, or len of question?
	if szx < 0 {
		// +2 = padding in (both sides)
		szx = szx*-1 + len(name) + 4
	}

	mxy, mxx := GetTerminalSize()
	assert(szx < mxx,
		spf("YorN:	szx (%d) bigger than the terminal (%d)",
		szx, mxx))
	assert(szy < mxy,
		spf("YorN:	szy (%d) bigger than the terminal (%d)",
		szy, mxy))

	scr := MakeWin(name,
		stdout, stdin,
		0+YORN_YOFF, szy,
		0+YORN_XOFF, szx,
	)
	defer wend(scr)
	wDrawBorderName(scr, '#')

	irn := 0
	for ;(ats[irn].x == 0 && len(options) > (irn+1)); {
		irn++
	}
	krn := irn
	rn := ats[irn]
	iret := 0

	for i := range options {
		if i != irn {
			wprint(scr, ats[i].y, ats[i].x, options[i])
		} else {
			wuprint(scr, ats[i].y, ats[i].x, COL_HL+options[i]+COL_NC)
		}
	}

	var k string
	for { // draw and color
		for i := range options {
			if i != irn {
				wprint(scr, ats[i].y, ats[i].x, options[i])
			} else {
				wuprint(scr, ats[i].y, ats[i].x, COL_HL+options[i]+COL_NC)
			}
		}
		wmove(scr, rn.y, rn.x-1)

		k = wgtk(scr)
		// move and choose
		if (k == "down" || k == "right") {
			krn = irn
			irn++
			for ;(ats[irn].x == 0 && len(options) > (irn+1)); {
				irn++
			}
			if ats[irn].x == 0 {
				irn = krn
				iret--
			}
			iret++
			rn = ats[irn]
		} else if (k == "up" || k == "left" ) && irn!=0 {
			krn = irn
			irn--
			for ;(ats[irn].x == 0 && irn != 0); {
				irn--
			}
			if ats[irn].x == 0 {
				irn = krn
				iret++
			}
			iret--
			rn = ats[irn]
		} else if k == "enter" || k == "space" {
			return iret
		}
	}
}

func pxRGB(px pixel) (string) {
	return spf("\x1b[38;2;%d;%d;%dm", px.R, px.G, px.B)
}

func pxSum(px pixel) (int) {
	return int(px.R)+int(px.G)+int(px.B)
}

