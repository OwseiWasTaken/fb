package main

include "gutil"

const (
	FT_bit = iota
	FT_byte = iota
	FT_bytes = iota

	Exp_C = iota
	Exp_Map = iota
)

var (
	ExtToName = []string{
		".bim", // BIt Map
		".bym", // ByTe Map
		".bysm", // ByTeS Map
	}
)

include "bitmap.go"

type FlMap interface {

}

func main() {
}

func _GetType(name string) (int) {
	PS(name)
	for i:=range ExtToName {
		if (ExtToName[i] == name) {return i}
	}
	return -1
}

func GetObjAndType(name string) (obj string, id int) {
	dot := strings.Index(name, ".")
	id = _GetType(name[dot:])
	obj = name[:dot]
	return
}
