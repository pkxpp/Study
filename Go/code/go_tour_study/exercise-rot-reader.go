package main

import (
	"io"
	"os"
	"strings"
)

type rot13Reader struct{
	r io.Reader
}

func rot13(c byte) byte{
	if c >= 65 && c <= 90 {
		c += 13
		if c > 90 {
			c = c - 90 + 65 - 1
		}
	}
	if c >= 97 && c <= 122 {
		c += 13
		if c > 122 {
			c = c - 122 + 97 - 1
		}
	}
	return c;
}

func (rotR rot13Reader) Read(b []byte) (n int, err error) {
	n, err = rotR.r.Read(b)
	if err != nil {
		return n, err
	}
	for i, v := range b {
		b[i] = rot13(v)
	}
	return n, nil
}

func main() {
	s := strings.NewReader("Lbh penpxrq gur pbqr!")
	r := rot13Reader{s}
	io.Copy(os.Stdout, &r)
}

/*sum:
 * 理解：io.Copy(os.Stdout, &r)可以简单的理解为把标准输出的缓存作为Read的参数，也就是Read会把strings中的内容写到标准输出缓存里面；
 * 因为要读取rot13Reader中的内容，所以会用到接口io.Reader的Read方法，而这个方法被自己重新写了，把rot13Reader中的内容先写到标准输出缓存，然后进行一次解密
 * Bingo！
 */