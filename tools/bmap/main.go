package main

Include "termin"

include "bitmap.go"
include "bytemap.go"
include "bytesmap.go"

const (
	FullChar = '@'
	EmptyChar = '.'
	sFullChar = "@"
	sEmptyChar = "."
	nc = "\x1b[38;2;255;255;255m"
	yellow = "\x1b[38;2;255;255;0m"
	blue = "\x1b[38;2;0;0;255m"
	Exp_C = iota
)

func main() {
	InitTermin()

	b:=ReadByteMap("daddy")
	b.Interact()

	StopTermin()
	exit(0)
}

// bytes -> int
func ParseBInt(len int, bts []int) (int) {
	var x int = 0
	var i int
	for i = 0; i < len; i++ {
		x+= 255*x+bts[i]
	}
	return x
}

// int -> bytes
func UnparseBInt(v int) ([]byte) {
	var ret []byte
	var xc byte = 255
	for i:=0;xc != 0;i++{
		xc = uint8(v>>(8*i))
		ret = append([]byte{xc}, ret...)
	}
	return ret[1:]
}

// implement \n option
// select box
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
		x += len(options[i])+1
	}
	x = 0
	for {
		wmove(win, win.LenY-2, 3)
		for i:=0; i<lo; i++ {
			if i != x {
				wwrite(win, spf("%s ", options[i]))
			} else {
				wwrite(win, spf("%s%s%s ", blue, options[i], nc))
			}
		}
		wDrawBorderName(win, '#')
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

// default drawCoords
// y/limy   x/limx
// all/szc  byte/szc
func DrawCoords (y, x, szc, szl int) {
	sll:=szc*szl
	wprint(Win, 1, 1, spf("%s%d/%d   %d/%d   ",
		yellow, y+1,szc,   x+1,szl,
	))
	wprint(Win, 2, 1, spf("%s%d/%d   %d/%d   %s",
		blue, y*szl+x+1,sll,   (y*szl+x)/8+1,sll/8, nc,
	))
}

func pow(base, power int) (int) {
	for i:=0;i<power;i++ {
		base = base * base
	}
	return base
}
