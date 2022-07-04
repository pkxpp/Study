
function testSetTimeout(){
	let arr = ['John', 'Grace', 'Bob'];
	let fn = function(info, index){
		setTimeout(()=>{
			console.log(info);
		}, index);
	}
	for (let i = 0; i < arr.length; ++i){
		let a = {'index': i, 'name': arr.slice(i, i+1)};
		fn(a, i);
	}
}
testSetTimeout();