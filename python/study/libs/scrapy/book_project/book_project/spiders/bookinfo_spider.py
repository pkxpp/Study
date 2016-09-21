# -*- coding: utf-8 -*

import scrapy
from book_project.items import BookItem

class BookInfoSpider(scrapy.Spider):
    name = "bookinfo"
    allowed_domains = ["allitebooks.cn", "amazon.cn"]
    # allowed_domains = ["allitebooks.com"]	
    start_urls = [
        "http://www.allitebooks.com/security/",
    ]

    def parse(self, response):
        print("2222222222222222222222222")	
        num_pages = int(response.xpath(u'//a[contains(@title, "Last Page →")]/text()').extract_first())
        print(num_pages)
        num_pages = 2		
        base_url = "http://www.allitebooks.com/security/page/{0}/"
        for page in range(1, num_pages):	
            yield scrapy.Request(base_url.format(page), dont_filter=True, callback=self.parse_page)

    def parse_page(self, response):
        print("333333333333333333")
        for sel in response.xpath('//div/article'):
            book_detail_url = sel.xpath('div/header/h2/a/@href').extract_first()
            yield scrapy.Request(book_detail_url, callback=self.parse_book_info)

    def parse_book_info(self, response):
        print("5555555555555555555")
        title = response.css('.single-title').xpath('text()').extract_first()
        isbn = response.xpath('//dd[2]/text()').extract_first()
        item = BookItem()
        item['title'] = title
        item['isbn'] = isbn
        print(555, title, isbn)
        amazon_search_url = 'https://www.amazon.cn/s/ref=nb_sb_noss?url=search-alias%3Daps&field-keywords=' + isbn
        print(666, amazon_search_url)
        # amazon_search_url = "http://www.baidu.com"
        yield scrapy.Request(amazon_search_url, callback=self.parse_price)
        # yield scrapy.Request(amazon_search_url, callback=self.parse_price, meta={ 'item': item })

    def parse_price(self, response):
        print("77777777777777777")
        item = response.meta['item']
        item['price'] = response.xpath('//span/text()').re(r'\$[0-9]+\.[0-9]{2}?')[0]
        yield item