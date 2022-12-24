package mapio

func export (mp FlMap, Exp_id int) () {
	Exp_Str := MapToExt[Exp_id]
	if (Exp_Str == "") { Exp_Str = MapToExt[mp.GetId()]}
	printf("export map %s (of type %s) to %s%s\n",
	mp.GetObj(), MapToName[mp.GetId()], mp.GetObj(), Exp_Str)
}

func load (filename string) (FlMap) {
	assert(exists(filename), spf("file %s doesn't exist", filename))
	var (
		bts = ReadFileBytes(filename)
		obj, id = GetObjAndType(filename)
	)
	assert(len(bts) > 8, "file too small") // w=4, h=4
	w := ParseBInt(bts[0:FILEINTSIZE])
	h := ParseBInt(bts[FILEINTSIZE:FILEINTSIZE*2])
	b := _LoadMap(obj, w, h, bts[8:], id)
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

