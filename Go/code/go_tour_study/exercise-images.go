package main

import (
	"image"
	"image/color"
	"golang.org/x/tour/pic"
)

type Image struct{}

func (img Image) ColorModel() color.Model {
	return image.NewRGBA(img.Bounds()).ColorModel()
}

func (img Image) Bounds() image.Rectangle {
	var w, h = 256, 256
	return image.Rect(0, 0, w, h)
}

func (img Image) At(x, y int) color.Color {
	return image.NewRGBA(img.Bounds()).At(x, y)
}

func main() {
	m := Image{}
	pic.ShowImage(m)
}