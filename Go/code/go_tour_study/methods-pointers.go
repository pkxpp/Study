package main

import(
	"fmt"
	"math"
)

type Vertex struct{
	X, Y float64
}

func (v Vertex) Abs() float64{
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

// func (v Vertex) Scale(f float64) {
func (v *Vertex) Scale(f float64) {
	v.X = v.X * f
	v.Y = v.Y * f
}

type PVertex *Vertex 
// test **T
func (v *PVertex) test() {
	
}

func main() {
	v := Vertex{3, 4}
	fmt.Println(v.Abs())
	v.Scale(10)
	fmt.Println(v.Abs())
}

/*summary
* 1.值传递和指针传递还是存在的，要注意！！！
*/