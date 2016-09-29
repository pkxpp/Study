// 初始化
mui.init();

var main = null;
var showMenu = false;
var menu = null;

// 所有方法都放到这里
mui.plusReady(function(){
	// 侧滑菜单
	main = plus.webview.getWebviewById(plus.runtime.appid);
	var menuoptions = {
		url: 'views/menu.html',
		id: 'menu',
		styles : {
			left:0,
			width:'70%',
			zindex:-1
		}
	};
	menu = mui.preload(menuoptions);
//	$(document).off('tap', '.btnMenu').on('tap', '.btnAdd', opMenu);
	mui('.mui-bar').on('tap', '.btnMenu', opMenu);
	main.addEventListener('maskClick', opMenu);
	mui.menu = opMenu;

	// 退出
	mui.back = function(){
//		if($('.adda').is(':hidden')){
//			hideAdd();	
//		}else if(showMenu){
//			closeMenu();
//		}else{
//			qiao.h.exit();
//		}
	};
});

function test(){
	console.log('11111111111');
}

// menu
function opMenu(){
	console.log("showMenu: ", showMenu);
	if(showMenu){
		closeMenu();
	}else{
		openMenu();
	}
}
function openMenu(){
//	if($('.adda').is(':visible')){
//		setColor("#333333");
		menu.show('none', 0, function() {
			main.setStyle({
				mask: 'rgba(0,0,0,0.4)',
				left: '70%',
				transition: {
					duration: 150
				}
			});
	
			showMenu = true;
		});
//	}
}
function closeMenu(){
//	setColor("#f7f7f7");
	main.setStyle({
		mask: 'none',
		left: '0',
		transition: {
			duration: 100
		}
	});
	
	showMenu = false;
	setTimeout(function() {
		menu.hide();
	}, 300);
}

// set color
//function setColor(color){
//	if(mui.os.ios && color) plus.navigator.setStatusBarBackground(color);
//}