package mapio

func export (mp FlMap, ExpId int) () {
	obj := mp.GetObj()
	id := mp.GetId()
	ExpStr := MapToExt[ExpId]
	if (ExpId == 0) { ExpStr = MapToExt[id] } // if MAP_ -> self id

	printf("exporting map %s (of type %s) to %s%s\n",
	obj, MapToName[id], obj, ExpStr)
	_ExportMap(obj, mp, ExpId)
}

func _ExportMap(obj string, mp FlMap, ExpId int) {
	var out []byte
	// write out[] with to "obj+exp-ext"
	// and panic if it doesn't work
	defer panic(os.WriteFile(obj+MapToExt[ExpId], out, 0644)) // 1X 2W 4R
	switch (ExpId) {
	case MAP_: // self
		// get bytes to save
		bts := mp.GetByteArr()
		// make buffer
		out = make([]byte, FILEINTSIZE*2+len(bts))
		// write sizes
		iw, ih := mp.GetSize()
		w := UnparseBInt(iw)
		h := UnparseBInt(ih)
		for i:=0; i<FILEINTSIZE; i++ {
			out[i] = w[i]
			out[i+FILEINTSIZE] = h[i]
		}
		// write byte array
		for i:=0; i<len(bts); i++ {
			out[FILEINTSIZE*2+i] = bts[i]
		}
		// write file
	default:
		die(spf("can't export bmap to %s (%d)", MapToName[ExpId], ExpId))
	}
}

func load (filename string) (FlMap) {
	assert(exists(filename), spf("file %s doesn't exist", filename))
	var (
		bts = ReadFileBytes(filename)
		obj, id = GetObjAndType(filename)
	)
	assert(len(bts) > FILEINTSIZE*2, "file too small") // w=4, h=4
	w := ParseBInt(bts[0:FILEINTSIZE])
	h := ParseBInt(bts[FILEINTSIZE:FILEINTSIZE*2])
	b := _LoadMap(obj, w, h, bts[8:], id) // get map-specific content
	return b
}

func _LoadMap(objname string, w, h int, cont []byte, id int) (FlMap) {
	switch (id) {
	case MAP_bit:
		b := MakeBitmap(objname, w, h)
		b.btcont = cont
		b.cont[0] = true
		for i := range cont {
			l:=ExpandByte(cont[i])
			for j := range l {
				b.cont[8*i+j] = l[j]
			}
		}
		return b
	case MAP_byte:
		b := MakeBytemap(objname, w, h)
		b.cont = cont
		return b
	case MAP_bytes:
		b := MakeBytesmap(objname, w, h)
		for i:=0;i<len(cont);i+=3 {
			b.cont[i/3] = pixel{cont[i], cont[i+1], cont[i+2]}
		}
		return b
	default:
		assert(false, spf("_LoadMap %s not implemented yet\n", MapToName[id]))
	}
	return nil
}

