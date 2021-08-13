package example

import (
	"fmt"
)

type MyChildStruct struct {
	Key   string
	Value string
}

type MyStruct struct {
	String string
	Int    int
	Child  []*MyChildStruct
}

func ReceiveScalar(s string) {
	fmt.Printf("ReceiveScalar: %s\n", s)
}

func ReturnScalar() string {
	return "myString"
}

func ReceiveStruct(s *MyStruct) {
	fmt.Printf("ReceiveStruct: %s, %d\n", s.String, s.Int)
	for i, child := range s.Child {
		fmt.Printf("Child[%d]: %s, %s\n", i, child.Key, child.Value)
	}
}

func ReturnStruct() *MyStruct {
	c := []*MyChildStruct{
		&MyChildStruct{"myKey1", "myValue1"},
		&MyChildStruct{"myKey2", "myValue2"},
	}
	return &MyStruct{"sample", 100, c}
}

func ReturnStructSlice() []*MyStruct {
	len := 2
	ss := []*MyStruct{}
	for i := 0; i < len; i++ {
		c := []*MyChildStruct{
			&MyChildStruct{fmt.Sprintf("myKey%d_1", i), fmt.Sprintf("myValue%d_1", i)},
			&MyChildStruct{fmt.Sprintf("myKey%d_2", i), fmt.Sprintf("myValue%d_2", i)},
		}
		ss = append(ss, &MyStruct{fmt.Sprintf("sample%d", i), i, c})
	}
	return ss
}

func ReceiveStructSlice(ss []*MyStruct) {
	for _, s := range ss {
		fmt.Printf("ReceiveStructSlice: %s, %d\n", s.String, s.Int)
		for i, child := range s.Child {
			fmt.Printf("Child[%d]: %s, %s\n", i, child.Key, child.Value)
		}
	}
}
