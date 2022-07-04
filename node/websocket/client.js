// client
const WebSocket = require("ws");
var ws=new WebSocket("ws://127.0.0.1:9001");
ws.onopen=function(e){
  console.log('open...', e);
  // setTimeout(()=>{
	  // console.log('主动关闭连接.');
	  // ws.close(1000, '客户端主动关闭连接');
  // }, 1000);
};
ws.onclose=function(e){
  console.log('close...', e.code, e.reason);
  // ws.close();
};