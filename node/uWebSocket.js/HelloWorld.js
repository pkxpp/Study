/* Minimal SSL/non-SSL example */

const uWS = require('uWebSockets.js');
const port = 9001;

const app = uWS./*SSL*/App({
  key_file_name: 'misc/key.pem',
  cert_file_name: 'misc/cert.pem',
  passphrase: '1234'
})

app.listen(port, (token) => {
  if (token) {
    console.log('Listening to port ' + port);
  } else {
    console.log('Failed to listen to port ' + port);
  }
});

app.get('/*', (res, req) => {
  res.end('Hello World!');
})



// app.ws(
	// '/*',
	// {
		// open: (ws, req) => {
			// console.log('11111111111111111111', ws);
		// },
		// close: (ws, code, msg) => {
			// if (code != 0) {
				// console.log("ws closed with unexpected code : ", code);
			// }
		// },
		// message: (ws, msg, isBinary) => {
			
		// }
	// }
// )