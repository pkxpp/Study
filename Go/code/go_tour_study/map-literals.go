package main

import "fmt"

type Vertex struct{
	Lat, Long float64
}

var m = map[string]Vertex{
	"Bell Labs": Vertex{
		40.68433, -74.39967,
	},
	"Google": Vertex{
		37.42202, -122.08408,
	},
}

func main(){
	fmt.Println(m)
}

// study
/*
1. map的定义带初始化。key可以用[key] = value的形式，也可以用 key : value的形式;
2. struct的类型是Vertex，所以在申明和定义的时候要用到Vertex这个自定义的类型关键字
3. struct中元素要有逗号隔开，最后一个元素也要加
*/