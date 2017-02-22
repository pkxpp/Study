package main

import (
	// "fmt"
	"golang.org/x/tour/reader"
)

type MyReader struct{}

// TODO: Add a Read([]byte) (int, error) method to MyReader
func (r MyReader) Read(b []byte) (n int , err error) {
	// 不会做！
	// 继续做
	// fmt.Printf("len = %d cap = %d\n", len(b), cap(b))
	// fmt.SPrintf(b, "%c")
	n = len(b)
	// n = cap(b) // cap can make panic: runtime error.
	for i := 0; i < n; i++ {
		b[i] = 'A'
	}
	return n, nil
}

func main() {
	reader.Validate(MyReader{})
}