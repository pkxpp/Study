// 初始化
mui.init();

var main = null;
var showMenu = false;
var menu = null;
var selectType = "";

// 所有方法都放到这里
mui.plusReady(function(){
	// 初始化数据库
	initialize()
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
//	document.on('tap', '.btnAdd', tstAddData);
	mui('.mui-plus').on('tap', '.btnAdd', tstAddData);
	mui('.mui-bar').on('tap', '.btnMenu', opMenu);
	main.addEventListener('maskClick', opMenu);
	mui.menu = opMenu;
	
	tstPicker();
	// 退出
//	mui.back = function(){
//		if($('.adda').is(':hidden')){
//			hideAdd();	
//		}else if(showMenu){
//			closeMenu();
//		}else{
//			qiao.h.exit();
//		}
//	};
	
});

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

mui('body').on('shown', '.mui-popover', function(e) {
	console.log('shown', e.detail.id);//detail为当前popover元素
});
mui('body').on('hidden', '.mui-popover', function(e) {
	console.log('hidden', e.detail.id);//detail为当前popover元素
});
	
function newaddEvent(){
	console.log("newaddEvent");
	if (selectType == "")
	{
		mui.toast("不允许为空");
		return false;
	}
	console.log(selectType);	
	
	// 存数据库
	add(selectType, 10);
}

function getDatabase() {
	//@参数1：数据库名称
	//@参数2：版本号
	//@参数3：对数据库的描述
	//@参数4：设置数据的大小
	//@参数5：回调函数
     return openDatabase("testdb", "1.0", "page", 1024*1024);
}

function initialize(){
    var db = getDatabase()
    db.transaction(function(tx){
    	tx.executeSql("CREATE TABLE IF NOT EXISTS testTable (name TEXT, value INT, primary key (name))");
    });
    
    console.log("initialize ... ");
}

function insert(cname, cvalue){
    var db = getDatabase();
    var res = true;
    db.transaction(function(tx){
        tx.executeSql('INSERT INTO testTable VALUES (?,?);',[cname, cvalue],
	        function(tx,results){
	        },function (tx, error){
	            res = false;
	        }
        );
    })

    return res;
}

function update(cname, cvalue){
    var db = getDatabase();
    var res = true;
    db.transaction(function(tx){
        tx.executeSql('UPDATE testTable SET value = ? WHERE name = ?;',[cvalue, cname],
	        function(tx,results){
				console.log("update successed.");
	        },function (tx, error){
	            res = false;
	        }
        );
    })

    return res;
}

function select(cname){
    var db = getDatabase();
	var value = 0;
    db.transaction(function(tx){
        tx.executeSql('SELECT value from testTable WHERE name = ?;',[cname],
	        function(tx,results){
	        	var len = results.rows.length;
	        	if (len > 0){
					value = results.rows.item(0).value;
//					console.log("len=" + len + ", value = " + value);
				}
	        },function (tx, error){
	        }
        );
    })
	console.log("value = " + value);
    return value;
}

function add(cname, cvalue){
    var db = getDatabase();
	var value = 0;
    db.transaction(function(tx){
        tx.executeSql('SELECT value from testTable WHERE name = ?;',[cname],
	        function(tx,results){
	        	var len = results.rows.length;
	        	console.log("len = " + len);
	        	if (len > 0){	// update database
	        		console.log("add --> update database");
					value = results.rows.item(0).value;
					console.log("len=" + len + "," + value);
					// 上面得到的value是string类型
					console.log(typeof(value));
					value = Number(value) + Number(cvalue);
					update(cname, value);
				}
	        	else{			// insert database
	        		console.log("add --> insert database.");
	        		insert(cname, Number(cvalue));
	        	}
	        },function (tx, error){
//				console.log("error ..................");
	        }
        );
    })
	console.log("value = " + value);
    return value;
}

function loadValue(){
	console.log("loadValue ... ");
    var db = getDatabase();
    db.transaction(function(tx){
    	tx.executeSql('SELECT * FROM testTable', [], 
    	function (tx, results) {
    		var htmlList = new Array();
		    var len = results.rows.length;
		    for (i = 0; i < len; i++){
				console.log(results.rows.item(i).name + ", " + results.rows.item(i).value);
		    }
		},
    	function (tx,error){
    	  	console.log("loadValue error.");
    	});
		
	})
}

function tstPicker(){
	//普通示例
	var userPicker = new mui.PopPicker();
	userPicker.setData([
		{
		value: '直接访问',
		text: '直接访问'
		}, 
		{
			value: '邮件营销',
			text: '邮件营销'
		}, 
		{
			value: '联盟广告',
			text: '联盟广告'
		}, 
		{
			value: '视频广告',
			text: '视频广告'
		}, 
		{
			value: '搜索引擎',
			text: '搜索引擎'
		}, 
	]);

	var pick = document.getElementById('showPicker');
	pick.addEventListener('tap', function(event) {
		userPicker.show(function(items) {
//			userResult.innerText = JSON.stringify(items[0]);
			//返回 false 可以阻止选择框的关闭
//			console.log(JSON.stringify(items[0]));
//			console.log(items[0].value);
			selectType = items[0].text;
			
			// test
			select(selectType);
		});
	}, false);
}

function clearTable()
{
	var db = getDatabase();
    db.transaction(function(tx){
    	tx.executeSql('DELETE FROM testTable', [], 
    	function (tx, results) {
    		var htmlList = new Array();
		    var len = results.rows.length;
		    for (i = 0; i < len; i++){
				console.log(results.rows.item(i).name + ", " + results.rows.item(i).value);
		    }
		},
    	function (tx,error){
    	  	console.log("loadValue error.");
    	});
		
	})
}


function tstAddData()
{
	console.log("tstAddData .....................");
	
	var arrData = [
		{
			value: 335,
			name: '直接访问'
		}, 
		{
			value: 310,
			name: '邮件营销'
		}, 
		{
			value: 234,
			name: '联盟广告'
		}, 
		{
			value: 135,
			name: '视频广告'
		}, 
		{
			value: 1548,
			name: '搜索引擎'
		}
	]
	for (var i = 0; i < arrData.length; i++)
	{
		insert(arrData[i].name, arrData[i].value);
	}
}
