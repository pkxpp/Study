// Echo2 prints its command-line arguments.
package main

import(
	"fmt"
	"os"
)

func main() {
	s, sep := "", ""
	for _, arg := range os.Args[1:] {
		s += sep + arg
		sep = " "
	}
	fmt.Println(s)

	// Exercise 1.2
	for index, arg := range os.Args[1:] {
		fmt.Println(index, arg)
	}
}