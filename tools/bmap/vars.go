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
)

type FlMap interface {
	GetId() (int)
	GetObj() (string)
	GetSize() (int,int)
	GetPixel(int, int) (pixel)
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
