// https://blog.csdn.net/qq_35606400/article/details/107532615

// 语法
/* arrayObject.sort(function(nextValue,currentValue){
// code …
}); */

var arr = [5,4,3,2,1,6,7,8,9];
// 倒序
arr.sort((a,b)=>{
	// console.log(a,b);// 可以看到第一次赋值结果是a = 4，b = 5
	if(a<b){
		return 1;
	}else if(a > b){
		return -1;
	}else{
		return 0;
	}
});

console.log("12ssss" + 5);
console.log(arr);
// [
  // 9, 8, 7, 6, 5,
  // 4, 3, 2, 1
// ]

// 遇到返回0的时候，排序是乱序的
function TestSort(){
	var arr = [
		{Name: "1", Value: 0},
		{Name: "2", Value: 0},
		{Name: "3", Value: 0},
		{Name: "4", Value: 0},
		{Name: "5", Value: 0},
		{Name: "6", Value: 0},
	];

	arr.sort((a,b)=>{
		return b.Value - a.Value;
	});

	console.log(arr);
}
// TestSort();

function TestArrayDelete()
{
	var arr = [1, 2, 3];
	for (let i = 0; i < arr.length; ++i)
	{
		arr.pop();
	}
	console.log(arr.length); // 1
}
TestArrayDelete();
