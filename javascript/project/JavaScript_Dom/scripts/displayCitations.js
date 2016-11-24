function displayCitations()
{
	if (!document.getElementsByTagName || !document.createElement ||
		!document.createTextNode) return false;
	
	// 取得所有引用
	var quotes = document.getElementsByTagName("blockquote");
	// 遍历所有引用
	for (var i = 0; i < quotes.length; ++i) {
		if (!quotes[i].getAttribute("cite")) continue;
		var url = quotes[i].getAttribute("cite");
		var quoteChildren = quotes[i].getElementsByTagName("*");
		if (quoteChildren.length < 1) continue;
		var elem = quoteChildren[quoteChildren.length - 1];
//		alert(quotes[i].lastChild.nodeType);
//		elem = quotes[i].lastChild;
		// 创建标记
		var link = document.createElement("a");
		var linkText = document.createTextNode("source");
		link.appendChild(linkText);
		link.setAttribute("href", url);
		var superscript = document.createElement("sup");
		superscript.appendChild(link);
		// 把标记添加到引用的最后一个元素节点
		elem.appendChild(superscript);
	}
}

addLoadEvent(displayCitations);