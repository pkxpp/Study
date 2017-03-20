// email project main.go
package main

import (
	"fmt"
	"net/smtp"

	"github.com/jordan-wright/email"
)

func main() {
	e := email.NewEmail()
	e.From = "minpage <minpage_kindle@126.com>"
	e.To = []string{"pkxpp@126.com"}
	e.Subject = "Test Subject"
	e.Text = []byte("Text Body is, of course, supported!")
	e.HTML = []byte("<h1>Fancy HTML is supported, too!</h1>")
	// 注意这里的Password填的是授权码才是成功，登陆密码反而是错误的
	err := e.Send("smtp.126.com:25", smtp.PlainAuth("", "minpage_kindle@126.com", "Password", "smtp.126.com"))
	fmt.Println(err)
	fmt.Println("Hello World!")
}
