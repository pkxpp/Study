var http = require('http');

	http.createServer(function (req, res) {
	  res.writeHead(200, {'Content-Type': 'text/plain'});
	  res.end('Hello World\n');
	}).listen(1337, "0.0.0.0");
	console.log('Server running at  port 10.20.126.34:1337 ');
