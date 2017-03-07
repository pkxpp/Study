// timefunction project timefunction.go
package timefunction

import (
	"fmt"
	"time"
)

func GetNextDay(nDays int, tm time.Time) string {
	hour := nDays * 24
	dura := time.Duration(hour) * time.Hour
	//	fmt.Println("duration: ", dura)
	tmNext := tm.Add(dura)
	//	fmt.Println(tm, tmNext)
	year := tmNext.Year()
	month := tmNext.Month()
	day := tmNext.Day()

	strNextDay := fmt.Sprintf("%d.%d.%d", year, month, day)
	return strNextDay
}

func Test() {
	tm2, _ := time.Parse("01.02.2006", "02.08.2015")
	fmt.Println(tm2)
}
