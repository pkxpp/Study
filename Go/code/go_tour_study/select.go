package main

import "fmt"

func fibonacci(c, quit chan int) {
	x, y := 0, 1
	fmt.Println("22222222222222")
	for {
		select{
		case c<-x:
			fmt.Println("333333333")
			x, y = y, x+y
		case <-quit:
			fmt.Println("quit")
			return
		}
	}
}

func main() {
	c := make(chan int)
	quit := make(chan int)
	go func() {
		fmt.Println("11111111111111")
		for i := 0; i < 10; i++ {
			fmt.Println(<-c)
		}
		quit <- 0
	}()
	fibonacci(c, quit)
}