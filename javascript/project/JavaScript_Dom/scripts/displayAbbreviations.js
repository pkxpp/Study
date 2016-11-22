function displayAbbreviations()
{
	if (!document.getElementsByTagName || !document.createElement ||
		!document.createTextNode) return false;
	
	// 取得所有缩略词
	var abbreviations = document.getElementsByTagName("abbr");
	if (abbreviations.length < 1) return false;
	var defs = new Array();
	
	// 遍历这些缩略词
	for (var i = 0; i < abbreviations.length; ++i) {
		var abbrCurrent = abbreviations[i];
		// 早期的版本没有<abbr>属性节点
		if (abbrCurrent.childNodes.length < 1) continue;
		var definition = abbrCurrent.getAttribute("title");
		var key = abbrCurrent.lastChild.nodeValue;
		defs[key] = definition;
	}
	
	// 创建定义列表
	var dlist = document.createElement("dl");
	for (key in defs) {
		var definition = defs[key];
		// 创建定义标题
		var dtitle = document.createElement("dt");
		var dtitleText = document.createTextNode(key);
		dtitle.appendChild(dtitleText);
		//create the definition description
		var ddesc = document.createElement("dd");
		var ddescText = document.createTextNode(definition);
		ddesc.appendChild(ddescText);
		// 把他们添加到定义列表
		dlist.appendChild(dtitle);
		dlist.appendChild(ddesc);
	}
	
	if (dlist.childNodes.length < 1) return false;
	// 创建标题
	var header = document.createElement("h2");
	var headerText = document.createTextNode("Abbreviations");
	// 把标题添加到页面主体
	header.appendChild(headerText);
	document.body.appendChild(header);
	// 把定义列表添加到页面主体
	document.body.appendChild(dlist);
}

addLoadEvent(displayAbbreviations);

