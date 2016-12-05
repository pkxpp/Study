function prepareSlideshow(){
	// Check DOM
	if (!document.getElementsByTagName) return false;
	if (!document.getElementById) return false;
	// check element.
	if (!document.getElementById("linklist")) return false;
	var slideshow = document.createElement("div");
	slideshow.setAttribute("id", "slideshow");
	var preview = document.createElement("img");
	preview.setAttribute("src", "../image/topic.gif");
	preview.setAttribute("alt", "building blocks of web design");
	preview.setAttribute("id", "preview");
	slideshow.appendChild(preview);
	var list = document.getElementById("linklist");
	insertAfter(slideshow, list);
	
//	preview.style.position = "absolute";
	// list
	var list = document.getElementById("linklist");
	var links = list.getElementsByTagName("a");
	// mouseover
	links[0].onmouseover = function(){
		moveElement("preview", -100, 0, 10);
	}
	links[1].onmouseover = function(){
		moveElement("preview", -200, 0, 10);
	}
	links[2].onmouseover = function(){
		moveElement("preview", -300, 0, 10);
	}
}
addLoadEvent(prepareSlideshow);