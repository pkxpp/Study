package main

import (
	"fmt"
	"sync"
)

type Fetcher interface{
	// Fetch returns the body of URL and
	// a slice of URLs found on that page.
	Fetch(url string) (body string, urls []string, err error)
}

// Crawl uses fetcher to recursively crawl
// pages starting with url, to a maximum of depth.
func Crawl(url string, depth int, fetcher Fetcher) {
	// TODO: Fetch URLs in parallel.
	// TODO: Don't fetch the same URL twice.
	// This implementation doesn't do either:
	if depth <= 0 {
		return
	}

	body, urls, err := fetcher.Fetch(url)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Printf("found: %s %q\n", url, body)
	for _, u := range urls {
		Crawl(u, depth-1, fetcher)
	}
	return
}

func main() {
	// Crawl("http://golang.org/", 4, fetcher)
	f := FetchedUrl{m: make(map[string]int)}
	ch := make(chan string)
	f.Crawl("http://golang.org/", 4, fetcher, ch)
	// for v := range ch {
	// 	fmt.Println(v)
	// }
}

// fakeFetcher is Fetcher that returns canned results.
type fakeFetcher map[string]*fakeResult

type fakeResult struct{
	body string
	urls []string
}

func (f fakeFetcher) Fetch(url string) (string, []string, error) {
	if res, ok := f[url]; ok {
		return res.body, res.urls, nil
	}
	return "", nil, fmt.Errorf("not found: %s\n", url)
}

///////////////////////////////////////////////
// use goroutine
type FetchedUrl struct {
	m map[string]int
	mux sync.Mutex
}

func (f *FetchedUrl) AddNewUrl(url string) bool {
	f.mux.Lock()
	// 就能够省掉在每个return的时候都加上mux.Unlock
	defer f.mux.Unlock()

	if f.IsCanAdd(url) {
		f.m[url]++
		return true
	}
	return false
}

func (f *FetchedUrl) IsCanAdd(url string) bool {
	if v, ok := f.m[url]; ok && v > 0 {
		fmt.Printf("url: [%s] is exist!\n", url)
		return false
	}
	return true
}

func (f *FetchedUrl) Crawl(url string, depth int, fetcher Fetcher, ch chan string) {
	fmt.Println("depth: ", depth, url)
	if depth <= 0 {
		return
	}

	if !f.AddNewUrl(url) {
		return
	}

	body, urls, err := fetcher.Fetch(url)
	if err != nil {
		fmt.Println(err)
		return
	}
	// fmt.Printf("found: %s %q\n", url, body)
	str := fmt.Sprintf("found: %s %q\n", url, body)
	fmt.Println(str)
	// ch<-str
	for _, u := range urls {
		fmt.Println(111, u)
		go f.Crawl(u, depth-1, fetcher, ch)
	}
	return
}

// fetcher is a populated fakeFetcher.
var fetcher = fakeFetcher{
	"http://golang.org/": &fakeResult{
		"The Go Programming Language",
		[]string{
			"http://golang.org/pkg/",
			"http://golang.org/cmd/",
		},
	},
	"http://golang.org/pkg/": &fakeResult{
		"Packages",
		[]string{
			"http://golang.org/",
			"http://golang.org/cmd/",
			"http://golang.org/pkg/fmt/",
			"http://golang.org/pkg/os/",
		},
	},
	"http://golang.org/pkg/fmt/": &fakeResult{
		"Package fmt",
		[]string{
			"http://golang.org/",
			"http://golang.org/pkg/",
		},
	},
	"http://golang.org/pkg/os/": &fakeResult{
		"Package os",
		[]string{
			"http://golang.org/",
			"http://golang.org/pkg/",
		},
	},
}