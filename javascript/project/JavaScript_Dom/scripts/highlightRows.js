function highlightRows(){
	if (!document.getElementsByTagName) return false;
	var rows = document.getElementsByTagName("tr");
	for (var i = 0; i < rows.length; ++i){
		rows[i].onmouseover = function() {
			this.style.fontWeight = "bold";
			console.log("move in.........................");
		}
		rows[i].onmouseout = function(){
			this.style.fontWeight = "normal";
			console.log("move out.........................");
		}
	}
}

addEventListener(highlightRows);