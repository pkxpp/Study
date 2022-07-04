//·þÎñÆ÷³ÌÐò
/*
var crypto=require('crypto');
var WS='258EAFA5-E914-47DA-95CA-C5AB0DC85B11';
require('net').createServer(function(o){
  var key;
  o.on('data',function(e){
    if(!key){
      //ÎÕÊÖ
      key=e.toString().match(/Sec-WebSocket-Key: (.+)/)[1];
      key=crypto.createHash('sha1').update(key+WS).digest('base64');
      o.write('HTTP/1.1 101 Switching Protocols\r\n');
      o.write('Upgrade: websocket\r\n');
      o.write('Connection: Upgrade\r\n');
      o.write('Sec-WebSocket-Accept: '+key+'\r\n');
      o.write('\r\n');
      //¹¹Ôì¶ÏÁ¬ÇëÇóµÄÊý¾Ý²¿·Ö£¬Ç°ÃæÁôÁ½×Ö½Ú´æ·Å×´Ì¬Âë
      var buf=new Buffer('\0\0º¢×Ó£¬µØÇòÌ«Î£ÏÕÁË£¬¿ì»Ø»ðÐÇÈ¥°É£¡');
      buf.writeUInt16BE(1000,0); //ÔÚÍ·Á½¸ö×Ö½ÚÐ´ÈëÒ»¸ö×´Ì¬Âë
      //·¢ËÍ¶ÏÁ¬ÇëÇó
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
	console.log('æ–°å¢žåŠ è¿žæŽ¥.');
	setTimeout(()=>{
		console.log('æœåŠ¡å™¨ä¸»åŠ¨æ–­å¼€è¿žæŽ¥')
		ws.end(1000, 'æœåŠ¡å™¨ä¸»åŠ¨æ–­å¼€è¿žæŽ¥.');
		// ws.close(1000, 'æœåŠ¡å™¨ä¸»åŠ¨æ–­å¼€è¿žæŽ¥.');
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
