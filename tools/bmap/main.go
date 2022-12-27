package main

var (
	debug bool = false
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
			assert(!asf, spf("no overwrite of input $%d→\"%s\"", i, argv[i]))
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

	if IptId == MAP_ {
		//TODO: remake YorN
		// use YorN to get ID of map
	}
	filename = objname+MapToExt[IptId]

	if (debug) {
		printf("open `%s` as %s\nm:'%s' c:'%s'\nout: %s as %s\n",
			filename, MapToName[IptId],
			bog(interact, "interact", "export").(string), bog(asf, "load", "create"),
			expto, MapToName[ExpId],
		)
	}

	var b FlMap
	if asf {
		b = load(objname+MapToExt[IptId])
	} else {
		b = MakeMap(filename, IptId, 11, 11)
	}

	InitTermin()
	//export(b, ExpId)
	scr := MakeWin("",
		stdout, stdin,
		0, Win.MaxY,
		0, Win.MaxX,
	)

	iEdit(b, scr)
	StopTermin()
}
