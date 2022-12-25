package main

include "vars.go"
include "io.go"
include "gutil"
include "bitmap.go"
include "bytemap.go"
include "bytesmap.go"

func main() {
	// flags
	var (
		interact = true
		asf = false // already saved file
		export = false // auto export
		ExpId int
		expto string
		objname string = "new"
	)

	for i:=0; i<len(argv); i++{
		if argv[i][0] != '-' {
			assert(!asf, spf("no overwrite of input $%d→\"%s\"", i, argv[i]))
			asf = true
			objname, ExpId = GetObjAndType(argv[i])
		} else if argv[i] == "-e" {
			i++
			if !(i < len(argv)) {continue}
			expto = argv[i]
			objname, ExpId = GetObjAndType(expto)
			export = true
		} else if argv[i] == "-s" { // static
			interact = false
		} else {
			die(spf("can't parse arg %d \"%s\"", i, argv[i]))
		}
	}

	PS("open at `", objname, "` i:", interact, " e:",asf, " do e:", export, " o:`",expto, "` otype:",MapToExt[ExpId])
	//b:=load("here.bim").(bitmap)
	//b.btcont[0] = 0xae
	//export(b, MAP_)
}
