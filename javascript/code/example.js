var http = require('http');

http.createServer(function (request, response) {
	response.writeHead(200, {'Content-Type':'text/plain'})
	response.end('Hello World 33\n');
// }).listen(8124, "10.20.72.41");
}).listen(8124, "127.0.0.1");

console.log('Server running at http://127.0.0.1:8124/');
