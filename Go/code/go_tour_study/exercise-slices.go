package main

import "golang.org/x/tour/pic"
import "fmt"

func Pic(dx, dy int) [][]uint8 {
	a := make([][]uint8, dy)
	for y := 0; y < dy; y++ {
		a[y] = make([]uint8, dx)
	}
	fmt.Printf("len = %d\n", len(a))
	return a;
}

func main() {
	pic.Show(Pic)
}
