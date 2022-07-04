//服务器程序
/*
var crypto=require('crypto');
var WS='258EAFA5-E914-47DA-95CA-C5AB0DC85B11';
require('net').createServer(function(o){
  var key;
  o.on('data',function(e){
    if(!key){
      //握手
      key=e.toString().match(/Sec-WebSocket-Key: (.+)/)[1];
      key=crypto.createHash('sha1').update(key+WS).digest('base64');
      o.write('HTTP/1.1 101 Switching Protocols\r\n');
      o.write('Upgrade: websocket\r\n');
      o.write('Connection: Upgrade\r\n');
      o.write('Sec-WebSocket-Accept: '+key+'\r\n');
      o.write('\r\n');
      //构造断连请求的数据部分，前面留两字节存放状态码
      var buf=new Buffer('\0\0孩子，地球太危险了，快回火星去吧！');
      buf.writeUInt16BE(1000,0); //在头两个字节写入一个状态码
      //发送断连请求
      o.write(encodeDataFrame({FIN:1,Opcode:8,PayloadData:buf}));
    };
  });
}).listen(8000);*/

/* Simple pub/sub broadcasting example */
const uWS = require('uWebSockets.js');
const port = 9001;

const app = uWS./*SSL*/App({
  key_file_name: 'misc/key.pem',
  cert_file_name: 'misc/cert.pem',
  passphrase: '1234'
}).ws('/*', {
  /* Options */
  compression: 0,
  maxPayloadLength: 16 * 1024 * 1024,
  idleTimeout: 10,

  /* Handlers */
  open: (ws, req) => {
    /* Let this client listen to topic "broadcast" */
    // ws.subscribe('broadcast');
	console.log('鏂板鍔犺繛鎺�.');
	setTimeout(()=>{
		console.log('鏈嶅姟鍣ㄤ富鍔ㄦ柇寮�杩炴帴')
		ws.end(1000, '鏈嶅姟鍣ㄤ富鍔ㄦ柇寮�杩炴帴.');
		// ws.close(1000, '鏈嶅姟鍣ㄤ富鍔ㄦ柇寮�杩炴帴.');
	}, 1000);
  },
  
  message: (ws, message, isBinary) => {
    /* Broadcast this message */
    // ws.publish('broadcast', message, isBinary);
	console.log('message......');
  },
  drain: (ws) => {

  },
  close: (ws, code, message) => {
    /* The library guarantees proper unsubscription at close */
	console.log('close........', code, message);
  }
}).any('/*', (res, req) => {
  res.end('Nothing to see here!');
}).listen(port, (token) => {
  if (token) {
    console.log('Listening to port ' + port);
  } else {
    console.log('Failed to listen to port ' + port);
  }
});
