package vars

// id
const (
	// offset for export {self, C}
	MAP_ = iota
	MAP_C
	MAP_bit
	MAP_byte
	MAP_bytes
)

const (
	FILEINTSIZE = 4
	SIZEOFBYTE = 8
	HERE = "\x1b[38;2;255;0;0mHERE\x1b[38;2;255;255;255m"
	EDITOR_YOFF = 2
	EDITOR_XOFF = 3
)

// cut down on interface funcs
// make map specific .interacts
type FlMap interface {
	GetId() (int) // get type
	GetObj() (string) // get name wo/ etx
	GetSize() (int,int)
	GetByteArr() ([]byte)
	GetPxArray() ([]pixel)
	SetPxArray([]pixel) ()
	// SelfToItst // self -> bytemap
	// ItstToSelf // bytemap -> self
}

var (
	MapToExt = []string{
		"", // self
		".c", // C code
		".bim", // BIt Map
		".btm", // ByTe Map
		".btsm", // ByTeS Map
	}
	MapToName = []string{
		"self", // self
		"c", // C code
		"bitmap", // BIt Map
		"bytemap", // ByTe Map
		"bytesmap", // ByTeS Map
	}
)

type pixel struct { R,G,B byte }

// bytes -> int
func ParseBInt(bts []byte) (int) {
	b := 0
	for i:=0;i<FILEINTSIZE;i++ {
		b += int(bts[i]<<(SIZEOFBYTE*(FILEINTSIZE-i-1)))
	}
	return b
}

// int -> bytes
func UnparseBInt(v int) ([]byte) {
	b := make([]byte, FILEINTSIZE)
	for i:=0;i<FILEINTSIZE;i++ {
		b[i] = byte(v>>(SIZEOFBYTE*FILEINTSIZE-i-1))
	}
	return b
}

func ExpandBytes(v []byte) (ret []bool) {
	b := make([]bool, len(v)*SIZEOFBYTE)
	for i := 0; i<len(v); i++ {
		e := ExpandByte(v[i])
		for j := 0; j < SIZEOFBYTE; j++ {
			b[i*SIZEOFBYTE+j] = e[j]
		}
	}
	return b
}

func ExpandByte(v byte) (ret []bool) {
	b := make([]bool, SIZEOFBYTE)
	for i := 0; i < SIZEOFBYTE; i++ {
		b[i] = v&(1<<(SIZEOFBYTE-i-1))!=0
	}
	return b
}

func CompressByte(b []bool) (ret byte) {
	ret = 0
	for i := range b {
		ret += bog(b[i], byte(1<<(len(b)-i-1)), byte(0)).(byte)
		//ret += bog(b[i], byte(1<<i), byte(0)).(byte)
	}
	return ret
}

func _GetType(name string) (int) {
	for i := range MapToExt {
		if (MapToExt[i] == name) {return i}
	}
	assert(false, spf("_GetType: can't find ext %s", name))
	return -1
}

func GetObjAndType(name string) (obj string, id int) {
	dot := strings.Index(name, ".")
	assert(dot > 0, "GetObjAndType: no '.' in filename")
	id = _GetType(name[dot:])
	obj = name[:dot]
	return
}

func MakeMap(filename string, id, w, h int) (FlMap) {
	var b FlMap = nil
	switch (id) {
	case MAP_bit:
		b = MakeBitmap(filename, w, h)
	case MAP_byte:
		b = MakeBytemap(filename, w, h)
	case MAP_bytes:
		b = MakeBytesmap(filename, w, h)
	default:
		die(spf("MakeMap can't create id %d", id))
	}
	return b
}

func PxMean(p pixel) (byte) {
	return byte((int(p.R)+int(p.G)+int(p.B))/3)
}

