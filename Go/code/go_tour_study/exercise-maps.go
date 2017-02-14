package main

import(
	"golang.org/x/tour/wc"
	"strings"
)

func WorldCount(s string) map[string]int{
	m := make(map[string]int)
	a := strings.Fields(s);
	for _, v := range(a) {
		_, ok := m[v]
		if ok {
			m[v] += 1;
		} 
		else {
			m[v] = 1;
		}
	}
	return m;
	// return map[string]int{"x": 1}
}

func main(){
	wc.Test(WorldCount)
}