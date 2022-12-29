package editor

var _dbg_prt_len int = 0
var pprt int
var col_prt string

//TODO: should i make a status window
func dbg (scr *Window, format string, stuff ...interface{}) {
	if format == "\\" {
		cleardbg(scr)
		wprint(scr, -1, _dbg_prt_len, spf("%v", stuff[0]))
	} else {
		msg := spf(format, stuff...)
		wprint(scr, -1, _dbg_prt_len, msg)
		_dbg_prt_len += len(msg)+1 // +1 for padding
	}
}

func cleardbg (scr *Window) {
	_dbg_prt_len = 0
	wDrawLine(scr, -1, ' ') // clear status line
}

func prtmap  (wmap *Window, pxmap []pixel) {
	for i := 0; i<wmap.LenY-2; i++ {
		wmove(wmap, i+1, 1)
		for j := 0; j<wmap.LenX-1; j++ {
			wwrite(wmap, pxRGB(pxmap[i*(wmap.LenX-1)+j])+"#")
		}
	}
	wwrite(wmap, RGB(255,255,255))
}

func prtpen  (scr *Window, pencil [8]pixel, pcho int, y int) {
	for i := range pencil {
		pprt = (i+pcho)%len(pencil)
		col_prt = pxRGB(pencil[pprt])

		if pxSum(pencil[pprt]) < 200 {
			col_prt = RGB(100,100,100)
		}

		wprint(scr, y+i, 0,
			col_prt + spf("%d:%02x%02x%02x", pprt+1,
			pencil[pprt].R, pencil[pprt].G, pencil[pprt].B,
		))
	}
	wwrite(scr, RGB(255,255,255))
}


func iEdit (mp FlMap, scr *Window) {
	var (
		wmap *Window
		k string
		y int
		mxy int
		mxx int
		x int
		pxmap []pixel = mp.GetPxArray()
		pencil [8]pixel
		pcho = 0
	)
	wDrawLine(scr, -2, '-')
	cleardbg(scr)


	scr.name = spf("edit map `%s`", mp.GetObj()+MapToExt[mp.GetId()])
	w, h := mp.GetSize()

	wmap = MakeWin("Map Viwer",
		stdout, nil,
		EDITOR_YOFF, w+EDITOR_YOFF+2,
		EDITOR_XOFF, h+EDITOR_XOFF+1,
	)
	mxy = wmap.LenY-3 // -2 for padding, -1 for len
	mxx = wmap.LenX-2 // -2 for padding, -1 for len

	//wput(wmap, 'o')


	if (debug) {
		pencil[0] = pixel{255,0,0}
		pencil[1] = pixel{0,255,0}
		pencil[2] = pixel{0,0,255}
		pencil[3] = pixel{255,255,0}
		pencil[4] = pixel{0,127,255}
	}


	// major draw cycle
	wDrawBorderName(scr, ' ')
	wDrawBorderName(wmap, 'o')
	prtpen( scr, pencil, pcho, wmap.MinY )
	prtmap( wmap, pxmap )

	dbg(scr, "\\", "")

	for {
		//{
		//	// major draw cicle
		//	// TODO: reduce this
		//	wDrawBorderName(scr, ' ')
		//	wDrawBorderName(wmap, 'o')
		//	wDrawLine(scr, 1, ' ')
		//	wDrawLine(scr, -2, '-')
		//	prtpen( scr, pencil, pcho, wmap.MinY )
		//	prtmap( wmap, pxmap )
		//}

		wmove(wmap, y+1, x+1)
		k = wgtk(scr)
		switch (k) {
		case "up", "k":
			if y != 0 { y-- }
		case "down", "j":
			if y != mxy { y++ }
		case "left", "h":
			if x != 0 { x-- }
		case "right", "l":
			if x != mxx { x++ }
		case "x":
			pcho=(pcho+1)%8
			prtpen( scr, pencil, pcho, wmap.MinY )
		case "z":
			pcho=(pcho+7)%8
			prtpen( scr, pencil, pcho, wmap.MinY )
		case "q":
			return
		}
	}
}

