package main

var (
	debug bool
)

include "gutil"
Include "termin"
include "vars.go"

include "io.go"

include "bitmap.go"
include "bytemap.go"
include "bytesmap.go"

include "editor.go"

func main() {
	// flags
	var (
		interact = true // interact/export map
		asf = false // load/create file

		expto string = "new"
		ExpId int = MAP_

		objname string = "new"
		IptId int = MAP_

		filename string
	)

	for i:=0; i<len(argv); i++{
		if argv[i][0] != '-' {
			assert(!asf, spf("no overwrite of input $%d→\"%s\"", i, argv[i]));
			asf = exists(argv[i])
			objname, IptId = GetObjAndType(argv[i])
			if (ExpId == MAP_) {ExpId = IptId}
		} else if argv[i] == "-e" {
			i++
			if !(i < len(argv)) {continue}
			expto = argv[i]
			expto, ExpId = GetObjAndType(expto)
			interact = false
		} else if argv[i] == "--debug" {
			debug = true
		} else {
			die(spf("can't parse arg %d \"%s\"", i, argv[i]))
		}
	}

	// init visual
	InitTermin()

	if IptId == MAP_ {
		IptId=YorN(-2, -1,
			"type of image for "+objname, "\n",
			"bit", "\n",
			"byte", "\n",
			"bytes", "\n",
		)+2 // +2  to skip MAP_, MAP_C
		clear()
	}
	filename = objname+MapToExt[IptId]

	if (debug) {
		printf("open `%s` as %s\nm:'%s' f:'%s'\nout: %s as %s\n",
			filename, MapToName[IptId],
			bog(interact, "interact", "export"),
			bog(asf, "load", "create"),
			expto, MapToName[ExpId],
		)
		oldinput("%")
		clear()
	}

	var b FlMap
	if asf {
		b = load(filename)
	} else {
		b = MakeMap(filename, IptId, DEFAULT_MAP_Y, DEFAULT_MAP_X)
	}

	//export(b, ExpId)
	scr := MakeWin("", // no need to wend, since this win is not temporary
		stdout, stdin,
		0, Win.MaxY,
		0, Win.MaxX,
	)
	iEdit(b, scr)
	StopTermin()
	exit(0)
}
