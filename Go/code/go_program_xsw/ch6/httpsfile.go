package main

import (
	"net/http"
)

func main() {
	h := http.FileServer(http.Dir("."))
	http.ListenAndServeTLS(":8001", "rui.crt", "rui.key", h)
}
