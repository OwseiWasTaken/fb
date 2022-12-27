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

func (b bytesmap) GetSize() (int,int) {
	return b.width, b.height
}

func (b bytesmap) GetByteArr() ([]byte) {
	ret := make([]byte, len(b.cont)*3)
	for i:=range b.cont {
		ret[i*3+0] = b.cont[i].R
		ret[i*3+1] = b.cont[i].G
		ret[i*3+2] = b.cont[i].B
	}
	return ret
}

func (b bytesmap) GetPxArray() ([]pixel) {
	return b.cont
}

func (b bytesmap) SetPxArray(p []pixel) {
	assert(len(p) == b.width*b.height, "bytesmap.SetPxArr len of px array and cont aren't equal")
	b.cont = p
}

