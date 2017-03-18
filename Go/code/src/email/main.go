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
	e.Send("smtp.126.com:994", smtp.PlainAuth("", "minpage_kindle@126.com", "Page+2404", "smtp.126.com"))
	fmt.Println("Hello World!")
}
