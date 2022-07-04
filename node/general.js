
/////////////////////////////////////////////
// only return one result, not var/let problem
var testReturn = function () {
	let arrLet = [];
	var arrVar = [];
	for (var i = 0; i < 2; ++i) {
		arrLet.push(i);
		arrVar.push(i);
	}
	return arrLet, arrVar;
}
// var a1, a2 = testReturn();
// let a3, a4 = testReturn();
// console.log(111, a1)
// console.log(222, a2)
// console.log(333, a3)
// console.log(444, a4)

///////////////////////////////////////////////////////
// hasOwnProperty
let testHasOwnProperty = function(){
	var obj = {};
	obj[1] = 'obj1';
	obj[3] = 'obj3';
	obj[5] = 'obj5';
	console.log(obj.hasOwnProperty(1), obj.hasOwnProperty('1'));
	
	for( let id in obj){
		console.log(id, obj[id], typeof(id));
	}
	
	var id = 3;
	console.log('id =' + id);
	console.log(id, typeof(id));
	
	let m = new Map;
	m.set(1, 'map1');
	m.set(3, 'map3');
	m.set(5, 'map5');
	m[2] = 'map2';
	m[4] = 'map4';
	m[6] = 'map6';
	console.log('---------------');
	for(let id in m){
		console.log(id, typeof(id))
	}
	for(let id of m.keys()){
		console.log(id, typeof(id))
	}
	
	console.log(m);
	console.log(m['4'], m[4]);
	console.log(m.get(2), m.get('4'));
	console.log(m.get(1), m.get('1'), m[1]);
	
	let arr = []
	arr.push(7);
	arr.push(8);
	arr.push(9);
	console.log('---------------');
	for(let i in arr){
		console.log(i, typeof(i));
	}
	for(let i of arr){
		console.log(i, typeof(i));
	}
}
testHasOwnProperty();