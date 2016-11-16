function showPic(whichpic){
	var source = whichpic.getAttribute("href");
	var placeholder = document.getElementById("placeholder");
	placeholder.setAttribute("src", source);
	
	// title
	var text = whichpic.getAttribute("title");
	var description = document.getElementById("description");
//	alert(description.nodeValue);	// null: 元素节点和文本节点不是一个节点, 下面才能获取正确的问题吧内容
//	alert(description.childNodes[0].nodeValue);	// 获取文本节点的内容
	description.firstChild.nodeValue = text;

	return false;
}


function countBodyChildren () {
	var body_element = document.getElementsByTagName("body")[0];
//	alert(body_element.childNodes.length);
//	alert(body_element.nodeType);

	for (var i  = 0; i < body_element.childNodes.length; ++i) {
		var nodeType = body_element.childNodes[i].nodeType;
		var nodeValue = body_element.childNodes[i].nodeValue;
//		console.log( nodeType + "," + nodeValue);
	}
}

window.onload = countBodyChildren();
