function prepareGallery(){
	
	
	if (!document.getElementsByTagName) return false;
	if (!document.getElementById) return false;
	if (!document.getElementById("imagegallery")) return false;
	var gallery = document.getElementById("imagegallery");
	var links = gallery.getElementsByTagName("a");
	for (var i = 0; i <links.length; ++i) {
		links[i].onclick = function() {
			console.log("2222222222222322222222222222222222");
			return !showPic(this);
		}
	}
}

function showPic(whichpic){
	if (!document.getElementById("placeholder")) return false;
	var source = whichpic.getAttribute("href");
	var placeholder = document.getElementById("placeholder");
	if (placeholder.nodeName != "IMG") return false;
	placeholder.setAttribute("src", source);
	
	// title
	if (document.getElementById("description")){
		var text = whichpic.getAttribute("title");
		var description = document.getElementById("description");
	//	alert(description.nodeValue);	// null: 元素节点和文本节点不是一个节点, 下面才能获取正确的问题吧内容
	//	alert(description.childNodes[0].nodeValue);	// 获取文本节点的内容
		if (description.firstChild.nodeType == 3){
			description.firstChild.nodeValue = text;
		}
	}

	return true;
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

function preparePlaceholder()
{
	var placeholder = document.createElement("img");
	placeholder.setAttribute("id", "placeholder");
	placeholder.setAttribute("src", "../image/all.jpg");
	placeholder.setAttribute("alt", "My gallery");
	var description = document.createElement("p");
	description.setAttribute("id", "description");
	var desctext = document.createTextNode("Choose an image");
	description.appendChild(desctext);
	
//	document.getElementsByTagName("body")[0].appendChild(placeholder);
//	document.body.appendChild(description);
	// insertBefore
	var gallery = document.getElementById("imagegallery");
	insertAfter(placeholder, gallery);
	insertAfter(description, placeholder);
}

//window.onload = prepareGallery;
function addLoadEvent (func) {
	var oldonload = window.onload;
	if (typeof window.onload != 'function'){
		window.onload = func;
	}
	else{
		window.onload = function() {
			oldonload();
			func();
		}
	}
}

function insertAfter(newElement, targetElement){
	var parent = targetElement.parentNode;
	if (parent.lastChild == targetElement){
		parent.appendChild(newElement);
	}else{
		parent.insertBefore(newElement, targetElement.nextSibling);
	}
}

addLoadEvent(preparePlaceholder);
addLoadEvent(prepareGallery);