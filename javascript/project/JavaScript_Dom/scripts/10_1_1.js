function positionMessage(){
	if (!document.getElementById) return false;
	if (!document.getElementById("message")) return false;
	var elem = document.getElementById("message");
	elem.style.position = "absolute";
	elem.style.left = "50px";
	elem.style.top = "100px";
	// 1：带有括号的函数接口；时间单位为毫秒
	movement = setTimeout("moveMessage()", 5000);
}

function moveMessage(){
	if (!document.getElementById) return false;
	if (!document.getElementById("message")) return false;
	var elem = document.getElementById("message");
//	elem.style.left = "200px";
	// 动画效果
	var xpos = parseInt(elem.style.left);
	var ypos = parseInt(elem.style.top);
	if (xpos == 200 && ypos == 100){
		return true;
	}
	if (xpos < 200){
		xpos++;
	}
	if (xpos > 200){
		xpos--;
	}
	if (ypos < 100){
		ypos++;
	}
	if (ypos > 100){
		ypos--;
	}
	elem.style.left = xpos + "px";
	elem.style.top = ypos + "px";
	movement = setTimeout("moveMessage()", 10);
}

function moveElement(elementID, final_x, final_y, interval){
	if (!document.getElementById) return false;
	if (!document.getElementById(elementID)) return false;
	var elem = document.getElementById(elementID);

	// 动画效果
	var xpos = parseInt(elem.style.left);
	var ypos = parseInt(elem.style.top);
	if (xpos == final_x && ypos == final_y){
		return true;
	}
	if (xpos < final_x){
		xpos++;
	}
	if (xpos > final_x){
		xpos--;
	}
	if (ypos < final_y){
		ypos++;
	}
	if (ypos > final_y){
		ypos--;
	}
	elem.style.left = xpos + "px";
	elem.style.top = ypos + "px";
	var repeat = "moveElement('" + elementID + "', " + final_x + ", " + final_y + ", " + interval + ")";
	movement = setTimeout(repeat, interval);
}
addLoadEvent(positionMessage);
//addLoadEvent(moveMessage);