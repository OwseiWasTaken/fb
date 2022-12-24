package Bitmap

type bitmap struct {
	filename string
	objname string
	cont []bool; btcont []byte
	width int
	height int
}

func MakeBitmap(name string, h, w int) (bitmap) {
	// when adapting, assert index != -1
	if strings.Index(name, ".") == -1 {name+=MapToExt[MAP_bit]}
	obj, _ := GetObjAndType(name)
	return bitmap{
		name,
		obj,
		make([]bool, w*8*h), make([]byte, w*h),
		w, h,
	}
}

func (b bitmap) GetId () (int) {
	return MAP_bit
}

func (b bitmap) GetObj () (string) {
	return b.objname
}
