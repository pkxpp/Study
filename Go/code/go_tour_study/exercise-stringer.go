package main

import "fmt"

type IPAddr [4]byte

// TODO: Add a "String() string" method to IPAddr
func (ip IPAddr) String() string {
	return fmt.Sprintf("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3])
}

func main() {
	hosts := map[string]IPAddr{
		"loopback": {127, 0, 0, 1},
		"googleDNS": {8, 8, 8, 8},
	}
	for name, ip := range hosts {
		fmt.Printf("%v: %v\n", name, ip)
	}
}

/*sum:
 * 1.String()接口有一个返回值，是string类型；
 * 2.可以理解为自定这个类型输出时候的样子，毕竟返回的是字符串，就是为了输出
 */