function positionMessage(){
	if (!document.getElementById) return false;
	if (!document.getElementById("message")) return false;
	var elem = document.getElementById("message");
	elem.style.position = "absolute";
	elem.style.left = "50px";
	elem.style.top = "100px";
	// 1：带有括号的函数接口；时间单位为毫秒
	moveElement("message", 200, 100, 10);
}
addLoadEvent(positionMessage);