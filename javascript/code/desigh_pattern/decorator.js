var WidgetBox = {
	createNew: function(){
		var widget = {};
		widget.OnMouseUp = function()
		{
			console.log("WidgetBox OnMouseUp();");
		}
		return widget;
	}
}

var Decorator = {
	createNew: function(){	
		var widget = {};
		widget.item = WidgetBox.createNew();
		widget.OnMouseUp = function()
		{
			item.OnMouseUp();
			console.log("Decorator OnMouseUp();");
		}
		return widget;
	}
}

var WidgetBoxDecorator = {
	createNew: function(){
		// 继承
		var widget = Decorator.createNew();
		widget.item = WidgetBox.createNew();
		widget.OnMouseUp = function()
		{
			console.log("WidgetBoxDecorator OnMouseUp();");
		}
		return widget;
	}
}

// old operation
console.log("Old operation:");
var item = WidgetBox.createNew();
item.OnMouseUp();

// new operation
console.log("New operation:");
var item1 = WidgetBoxDecorator.createNew();
item1.OnMouseUp();
