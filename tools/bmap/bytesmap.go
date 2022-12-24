package Bitmap

type bytesmap struct {
	filename string
	objname string
	cont []pixel
	width int
	height int
}

func MakeBytesmap(name string, h, w int) (bytesmap) {
	// when adapting, assert index != -1
	if strings.Index(name, ".") == -1 {name+=MapToExt[MAP_bytes]}
	obj, _ := GetObjAndType(name)
	return bytesmap{
		name,
		obj,
		make([]pixel, w*h),
		w, h,
	}
}

func (b bytesmap) GetId () (int) {
	return MAP_bytes
}

func (b bytesmap) GetObj () (string) {
	return b.objname
}
