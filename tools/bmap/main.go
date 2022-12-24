package main

include "vars.go"
include "io.go"
include "gutil"
include "bitmap.go"
include "bytemap.go"
include "bytesmap.go"

func main() {
	PS(load("here.bim"))
	PS(load("here.btm"))
	PS(load("here.btsm"))
}
