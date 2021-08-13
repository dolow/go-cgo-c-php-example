package main

// #include <stdlib.h>
// #include <main.h>
import "C"
import (
	"unsafe"

	example "dolow/go-cgo-c-php-example-go"
)

//export ReceiveScalar
func ReceiveScalar(s *C.char) {
	example.ReceiveScalar(C.GoString(s))
}

//export ReturnScalar
func ReturnScalar() *C.char {
	return C.CString(example.ReturnScalar())
}

//export ReceiveStruct
func ReceiveStruct(csa *C.MyStruct) {
	gos := &example.MyStruct{}
	cMyStructToGoMyStruct(csa, gos)
	example.ReceiveStruct(gos)
}

//export ReturnStruct
func ReturnStruct() *C.MyStruct {
	gos := example.ReturnStruct()
	cs := newCMyStruct()

	goMyStructToCMyStruct(gos, cs)

	return cs
}

//export ReceiveStructArray
func ReceiveStructArray(css *C.MyStruct) {
	csRefs := (*[1 << 32]C.MyStruct)(unsafe.Pointer(css))
	goss := []*example.MyStruct{}

	for i := 0; csRefs[i].string != nil; i++ {
		gos := &example.MyStruct{}
		cMyStructToGoMyStruct(&csRefs[i], gos)
		goss = append(goss, gos)
	}

	example.ReceiveStructSlice(goss)
}

//export ReturnStructArray
func ReturnStructArray() *C.MyStruct {
	goss := example.ReturnStructSlice()

	css := newCMyStructArray(len(goss) + 1)
	refs := (*[1 << 32]C.MyStruct)(unsafe.Pointer(css))

	for i, s := range goss {
		refs[i] = C.MyStruct{}
		goMyStructToCMyStruct(s, &refs[i])
	}
	refs[len(goss)] = C.MyStruct{nil, 0, nil}

	return css
}

func cMyStructToGoMyStruct(sc *C.MyStruct, sgo *example.MyStruct) {
	sgo.String = C.GoString(sc.string)
	sgo.Int = int(sc.integer)

	if sc.child == nil {
		return
	}

	sgo.Child = []*example.MyChildStruct{}

	cChildRefs := (*[1 << 32]C.MyChildStruct)(unsafe.Pointer(sc.child))

	for i := 0; ; i++ {
		cChildRef := cChildRefs[i]
		if cChildRef.key == nil {
			break
		}

		sgo.Child = append(sgo.Child, &example.MyChildStruct{
			C.GoString(cChildRef.key),
			C.GoString(cChildRef.value),
		})
	}
}

func goMyStructToCMyStruct(sgo *example.MyStruct, sc *C.MyStruct) {
	sc.string = C.CString(sgo.String)
	sc.integer = C.int(sgo.Int)

	if sgo.Child == nil {
		return
	}

	sc.child = newCMyChildStructArray(len(sgo.Child) + 1)
	cChildRefs := (*[1 << 32]C.MyChildStruct)(unsafe.Pointer(sc.child))

	for i, child := range sgo.Child {
		cChildRefs[i].key = C.CString(child.Key)
		cChildRefs[i].value = C.CString(child.Value)
	}

	cChildRefs[len(sgo.Child)].key = nil
	cChildRefs[len(sgo.Child)].value = nil
}

func newCMyStruct() *C.MyStruct {
	return (*C.MyStruct)(C.malloc(C.size_t(unsafe.Sizeof(C.MyStruct{}))))
}

func newCMyStructArray(length int) *C.MyStruct {
	return (*C.MyStruct)(C.malloc(C.size_t(unsafe.Sizeof(C.MyStruct{})) * C.ulong(length)))
}

func newCMyChildStructArray(length int) *C.MyChildStruct {
	return (*C.MyChildStruct)(C.malloc(C.size_t(unsafe.Sizeof(C.MyChildStruct{})) * C.ulong(length)))
}

func main() {
	panic("don't execute this")
}
