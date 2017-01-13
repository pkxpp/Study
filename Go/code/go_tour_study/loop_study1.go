package main

import (
	"fmt"
	"math"
)

func Sqrt(x float64) float64 {
	z := x/2
	tmp := 0.0
	// for i :=1; i < 10; i++ {
	fmt.Println("1111", z-tmp, math.Abs(z - tmp) < 0.000001)
	for math.Abs(z - tmp) > 0.0001 {
		tmp = z
		z = z - (z*z - x)/(2*z)
		fmt.Println(math.Abs(z - tmp), z, tmp, z - tmp)
	}
	return z;
}

func main() {
	fmt.Println(Sqrt(2))
}