function displayAccessKeys(){
	if (!document.getElementsByTagName || !document.createElement ||
		!document.createTextNode) return false;
	
	// 取得所有链接
	var links = document.getElementsByTagName("a");
	// 创建一个数组，保存访问键
	var akeys = new Array();
	// 遍历链接
	for (var i = 0; i < links.length; ++i) {
		var currentLink = links[i];
		if (!currentLink.getAttribute("accesskey")) continue;
		var key = currentLink.getAttribute("accesskey");
		var text = currentLink.lastChild.nodeValue;
		akeys[key] = text;
	}
	
	// 创建列表
	var list = document.createElement("ul");
	// 遍历访问键
	for (key in akeys) {
		var text = akeys[key];
		var str = key + ": " + text;
		var item = document.createElement("li");
		var itemText = document.createTextNode(str);
		item.appendChild(itemText);
		list.appendChild(item);
	}
	// 创建标题
	var header = document.createElement("h3");
	var headerText= document.createTextNode("Accesskeys");
	header.appendChild(headerText);
	document.body.appendChild(header);
	document.body.appendChild(list);
}

addLoadEvent(displayAccessKeys);

