function styleHeaderSiblings(){
	if (!document.getElementsByTagName) return false;
	var headers = document.getElementsByTagName("h1");
	var elem;
	for (var i = 0; i < headers.length; ++i) {
		elem = getNextElement(headers[i].nextSibling);
		
//		elem.style.fontWeight = "bold";
//		elem.style.fontSize = "1.2em";
//		elem.className = "intro";
		addClass(elem, "intro");
	}
}


function getNextElement(node){
	if (node.nodeType == 1){
		return node;
	}
	if (node.nextSibling){
		return getNextElement(node.nextSibling);
	}
	return null;
}

function addClass(element, value){
	if (!element.classname){
		element.classname = value;
	}else{
		newClassName = element.classname;
		newClassName += " ";
		element.classname = newClassName;
	}
}

addLoadEvent(styleHeaderSiblings);
