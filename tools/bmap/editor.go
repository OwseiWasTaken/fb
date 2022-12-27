package editor

var _dbg_prt_len int

func iEdit (mp FlMap, scr *Window) {
	dbg := func (format string, stuff ...interface{}) {
		msg := spf(format, stuff...)
		wprint(scr, -1, _dbg_prt_len, msg)
		_dbg_prt_len += len(msg)+1 // +1 for padding
	}
	cleardbg := func () {
		_dbg_prt_len = 0
		wprint(scr, -1, 0, "")
	}
	cleardbg()


	scr.name = spf("edit map [%s]", mp.GetObj())
	w, h := mp.GetSize()

	wmap := MakeWin("Map Viwer",
		stdout, nil,
		EDITOR_YOFF, w+EDITOR_YOFF+2,
		EDITOR_XOFF, h+EDITOR_XOFF+1,
	)

	//wput(wmap, 'o')

	wDrawLine(scr, -1, ' ')
	wDrawLine(scr, -2, '-')
	// why wput here?

	dbg(":")
	PS(w, h)
	wDrawBorderName(wmap, 'o')

	wgtk(scr)
}

func YorN (options ...string) (string) {
	jmp := make([]bool, len(options))
	var qst []string = {options[0]}
	//TODO finish
}

