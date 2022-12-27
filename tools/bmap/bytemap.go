package Bytemap
type bytemap struct {
	filename string
	objname string
	cont []byte
	width int
	height int
}

func MakeBytemap(name string, h, w int) (bytemap) {
	// when adapting, assert index != -1
	if strings.Index(name, ".") == -1 {name+=MapToExt[MAP_byte]}
	obj, _ := GetObjAndType(name)
	return bytemap{
		name,
		obj,
		make([]byte, w*h),
		w, h,
	}
}

func (b bytemap) GetId () (int) {
	return MAP_byte
}

func (b bytemap) GetObj () (string) {
	return b.objname
}

func (b bytemap) GetSize() (int,int) {
	return b.width, b.height
}

func (b bytemap) GetByteArr() ([]byte) {
	return b.cont
}

func (b bytemap) GetPxArray() ([]pixel) {
	var ret []pixel = make([]pixel, b.width*b.height)
	for i := range b.cont {
		ret[i] = pixel{b.cont[i], b.cont[i], b.cont[i]}
	}
	return ret
}

func (b bytemap) SetPxArray(p []pixel) {
	assert(len(p) == b.width*b.height, "bytemap.SetPxArr len of px array and cont aren't equal")
	for i:=range p {
		b.cont[i] = PxMean(p[i])
	}
}

