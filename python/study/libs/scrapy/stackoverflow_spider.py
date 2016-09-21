import scrapy


class StackOverflowSpider(scrapy.Spider)
    name = 'stackoverflow'
    start_urls = ['httpstackoverflow.comquestionssort=votes']

    def parse(self, response)
        for href in response.css('.question-summary h3 aattr(href)')
            full_url = response.urljoin(href.extract())
            yield scrapy.Request(full_url, callback=self.parse_question)

    def parse_question(self, response)
        yield {
            'title' response.css('h1 atext').extract_first(),
            'votes' response.css('.question .vote-count-posttext').extract_first(),
            'body' response.css('.question .post-text').extract_first(),
            'tags' response.css('.question .post-tagtext').extract(),
            'link' response.url,
        }