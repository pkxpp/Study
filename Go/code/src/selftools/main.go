// selftools project main.go
package main

import (
	"fmt"
	"time"
	"timefunction"
)

func main() {
	days := []int{1, 2, 6, 14, 29}
	tm := time.Now()
	//	tm, _ := time.Parse("07.03.2007", "07.03.2007")
	for _, v := range days {
		fmt.Println(timefunction.GetNextDay(v, tm))
	}
}
