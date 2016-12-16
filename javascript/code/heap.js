/******************************************************
* file    : 堆
* author  : "page")
* time    : "2016/11/02")
*******************************************************/

// 参考：http://www.cnblogs.com/yc_sunniwell/archive/2010/06/28/1766751.html

function Heap()
{
	this.data = [];
}

Heap.prototype.print = function () {
	console.log("Heap: " + this.data);
}

Heap.prototype.build = function(data){
	// 初始化
	this.data = [];
	if (!data instanceof Array)
		return false;

	// 入堆
	for (var i = 0; i < data.length; ++i) {
		this.insert(data[i]);
	}

	return true;
}

Heap.prototype.insert = function( nValue ){
	if (!this.data instanceof Array) {
		this.data = [];
	}

	this.data.push(nValue);
	// console.log("nValue: " + nValue);
	// this.print();

	// 更新新节点
	var nIndex = this.data.length-1;
	var nFatherIndex = Math.floor((nIndex-1)/2);
	while (nFatherIndex > 0){
		// console.log("[nIndex:" + nIndex + "] = " + this.data[nIndex] + "," + "[nFatherIndex:" + nFatherIndex + "] = " + this.data[nFatherIndex])
		if (this.data[nIndex] < this.data[nFatherIndex]) {
			var temp = this.data[nIndex];
			this.data[nIndex] = this.data[nFatherIndex];
			this.data[nFatherIndex] = temp;
		}

		nIndex = nFatherIndex;
		nFatherIndex = Math.floor((nIndex-1)/2);
	}

	// console.log("After push: " + nValue);
	// this.print();
}

Heap.prototype.delete = function(  ){
	if (!this.data instanceof Array) {
		return null;
	}

	var nIndex = 0;
	var nValue = this.data[nIndex];
	var nMaxIndex = this.data.length-1;
	// 更新新节点
	var nLeaf = this.data.pop();
	this.data[nIndex] = nLeaf;

	while (nIndex < nMaxIndex ){
		var nLeftIndex = 2 * (nIndex+1) - 1;
		var nRightIndex = 2 * (nIndex+1);

		// 找最小的一个子节点(nLeftIndex < nRightIndex)
		var nSelectIndex = nLeftIndex;
		if (nRightIndex < nMaxIndex) {
			nSelectIndex = (this.data[nLeftIndex] > this.data[nRightIndex]) ? nRightIndex : nLeftIndex;
		}

		if (nSelectIndex < nMaxIndex && this.data[nIndex] > this.data[nSelectIndex] ){
			var temp = this.data[nIndex];
			this.data[nIndex] = this.data[nSelectIndex];
			this.data[nSelectIndex] = temp;
		}

		nIndex = nSelectIndex;
	}

	return nValue;
}

// test
var heap = new Heap();
heap.build([1, 3, 5, 11, 4, 6, 7, 12, 15, 10, 9, 8]);
heap.print();

// insert
// heap.insert(2);
// heap.print();

// delete
heap.delete();
heap.print();
