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

func (b bitmap) GetSize() (int,int) {
	return b.width, b.height
}

func (b bitmap) GetByteArr() ([]byte) {
	// get cont/8 + if %8 != 0
	ret := make([]byte, len(b.cont)/8)
	cpy := b.cont
	// if there aren't %8 bits
	if len(b.cont)%SIZEOFBYTE!=0 {
		ret = append(ret, 0)
		for i:=0;i<SIZEOFBYTE-(len(b.cont)%SIZEOFBYTE);i++ {
			cpy = append(cpy, false)
		}
	}
	for i := 0; i<len(ret); i++ {
		ret[i] = CompressByte(cpy[SIZEOFBYTE*i:SIZEOFBYTE*(i+1)])
	}
	return ret
}
