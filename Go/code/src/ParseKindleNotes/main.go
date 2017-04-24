// ParseKindleNotes project main.go
package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"
)

func ReadLine(fileName string, handler func(string)) error {
	f, err := os.Open(fileName)
	if err != nil {
		return err
	}
	buf := bufio.NewReader(f)
	for {
		line, err := buf.ReadString('\n')
		line = strings.TrimSpace(line)
		handler(line)
		if err != nil {
			if err == io.EOF {
				return nil
			}
			return err
		}
	}
	return nil
}

func main() {
	ReadLine("Kindle-Notebook.csv", ParseWord)
}

func ParseWord(line string) {
	strs := strings.Split(line, ",")
	if len(strs) >= 4 {
		word := strings.Trim(strs[3], "\"")
		// construct
		newLine := "|" + word + "|"
		fmt.Println(newLine)
	}
}
