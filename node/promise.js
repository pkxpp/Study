function doActionList(list, index) {
	console.log('doActionList.............', index);
	return new Promise(resolve => {
		if(index >= list.length) {
			return resolve("done");
		} else {
			list[index]()
			doActionList(list, index+1).then(res => resolve(res));
		}
	})
}

// doActionList([10, 9, 8, 7, 6], 0).then(res=>{
	// console.log(res);
// })


var func = [
	function(){
		
		console.log('function 1');
	},
	function(){
		console.log('function 2');
	},
]


// doActionList(func, 0).then(res=>{
	// console.log(res);
// })


function delay(sec) {
  return new Promise(resolve => {
    console.log(`waiting ${sec}s...`, new Date().toLocaleString());
    setTimeout(() => resolve(), sec);
  });
}

var dramas = [
	{
		"time": 2000,
		"func": function(sec){
		
			console.log('function 1', new Date().toLocaleString());
			return new Promise(resolve => {
				// console.log(`1 waiting ${sec}s...`, new Date().toLocaleString());
				setTimeout(() => resolve(1), sec);
			});
		},
	},
	
	{
		"time": 3000,
		"func": function(sec){
		
			console.log('function 2', new Date().toLocaleString());
			return new Promise(resolve => {
				// console.log(`2 waiting ${sec}s...`, new Date().toLocaleString());
				setTimeout(() => resolve(2), sec);
			});
		},
	},
]

function doDrama(listDrama, index) {
  return new Promise((resolve, reject) => {
    if(index >= listDrama.length) {
      return resolve({ done: true, data: "done"});
    } else {
      return resolve({ done: false });
    }
  })
  .then(res => {
    if(res.done) {
      return Promise.resolve(res.data); 
    } else {
	  return listDrama[index].func(listDrama[index].time).then(()=>{
		  doDrama(listDrama, index+1);
	  })
    }
  });
}

// doDrama(dramas, 0).then(res=>{
	// console.log('done.')
// })


function test(){
	let counter = 0;
	const max = 5;

	function waitting(sec) {
	  return new Promise(resolve => {
		console.log(`waitting waiting ${sec}s...`);
		setTimeout(() => resolve(), 1000);
	  });
	}

	function promise() {
		console.log(`promise waiting ${++counter}s...`, new Date().toLocaleString());
		return new Promise(resolve => {
			if(counter >= max) {
				counter = 0;
				return resolve("promise finish");
			} else {
				setTimeout(() => {
					promise().then(res => resolve(res));
				}, 1000);
			}
		})
	}

	function promiseElse() {
		counter++;
		return new Promise((resolve, reject) => {
			if(counter >= max) {
				return resolve({ isValid: true, data: "promiseElse finish"});
			} else {
				return resolve({ isValid: false });
			}
		})
		.then(res => {
			if(res.isValid) {
				counter = 0;
				return Promise.resolve(res.data); 
			} else {
				return waitting(counter).then(() => promiseElse());
				// console.log('111 = ', t)
				// return t;
			}
		});
	}


	// promise()
	// .then(res => {
		// console.log(res);

		// return promiseElse();
	// })
	// .then(res => {
		// console.log(res);
	// });
	promiseElse()
	.then(res=>{
		console.log(res);
	})
	
	// 问题1：如何保证promiseElse finish只有最后一次才打印？
	/*
	 * (1)好复杂的问题
	 * (2)promiseElse返回的是一个Promise，所以才有.then执行，打印最后一条打印
	 * (3)promiseElse返回的Promise是哪一个呢？非常深的坑。首先，不是new Promise这个对象。而是new Promise.then返回的Promise对象
	 * (4)new Promise.then返回的对象是哪一个？如果递归结束，那么就是finish这个，没问题
	 * (5)如果，递归没有结束。返回的是waitting.then函数返回的Promise对象。而这个对象函数体是有返回的，并且是Promise对象，所以，最终是递归调用promiseElse()返回的Promise对象。因此，return waitting(counter).then(() => promiseElse());这句话返回的Promise对象，就相当于return promiseElse()返回的Promise对象。而这会再次执行promiseElse()函数，实现了递归。
	 */
	// 问题2：promiseElse里面的then和外面的then执行顺序？
	// 问题3：then的输入参数，即res，是从哪里给的？
	// 问题4：return返回promise和promise.then是一样的吗？
}
// test();

// promise执行顺序
function testThenOrder(){
	new Promise((resolve, reject) => {
		console.log("promise")
		resolve()
	})
    .then(() => {		// 执行.then的时候生成一个promise是给最后一个.then的
        console.log("then1")
        // return new Promise((resolve, reject) => {
        new Promise((resolve, reject) => {
            console.log("then1promise")
            resolve()
        })
		.then(() => {// 执行这个.then的时候，生成的promise是下面一个then的
			console.log("then1then1")
		})
		.then(() => {
			console.log("then1then2")
		})
    })
    .then(() => {
		// 这个
        console.log("then2")
    })
}
// testThenOrder();

// promise
// then 1
// then 1 promise
// then1 then1
// then 2
// then1 then2

/* 小结：参考https://www.jianshu.com/p/aa3d8b3adde3
 * 1. 理论1：promise的then/catch方法执行后返回的也是一个promise，所以多个同级then的时候，是要等前一个then返回的promise被resolve
 * 2. 理论2：then函数返回值，参考：https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Promise/then
then函数返回的promise没有resolve，那么then执行完毕会返回同步返回值，(错误理解：简单理解就是then执行完的时候)，它自己返回的这个Promise被resolve。看到then2是在then1 then1之后才打印。如果代码改成return new Promise，那么就会在then1 then1之前打印。因为在执行then1的Promise同步代码的时候，已经resovle了，那么then1返回的promise就编程这个promise了，也就被resolve了，这个时候then2的then就放到队列里面了。
 * 3. 理论3：同步执行完，再执行队列里面的回调函数体，先执行timeTick（即then函数的参数所指的函数体），再执行timeout函数设置的回调函数体
 队列的概念：then1当中的Promise被resolve时，then1 then1会被注册放到队列中。等then1 then1函数体执行完毕的时候

 * 分析上面的打印结果
 * 1.Promise对象的理解：是一个对象，包含：状态(fulfilled, pending, rejected)，回调函数队列(pending状态下放在这里的then回调函数体)，自己的函数体
 * 2.Promise对象new的时候会同步执行自己的函数体，所以先打印promise
 * 3.resolve，resolve是会改变Promise对象状态的，从pending改成fulfilled
 * 4.同步执行第一个.then函数，这里注意.then函数是一个函数，他自己本身在同步的时候就执行了，只是她的回调函数体（参数）还没有执行而已。.then函数会根据她所属的Promise状态来做不同的事情。如果Promise状态是fulfilled就把回调函数体放到timeTick队列里面，如果是pending状态，就把函数体放在Promise自己的队列里面。所以第一个then执行的时候，她的函数体已经放在了Tick队列里面。并且.then函数有返回，参考https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Promise/then。因为.then函数的函数体都还没有执行，所以返回一个pending状态的Promise
 5.同步执行第二个.then函数，注意第二个.then函数是根据第一个.then函数返回的Promise对象的状态做决定的。因为是pending，所以这个.then函数的回调函数体放在了第一个.then函数返回的Promise对象的队列中
 6.下一个循环执行Tick队列，也就是第一个.then函数的函数体。
 (1)先同步执行一个new的Promise对象函数体，打印'then 1 promise'.并且这个Promise对象的状态被resolve切换为fulfilled
 (2)同步执行then1then1这个.then函数，并且把这个放在timeTick队列中，并且返回的Promise直接为fulfilled，但是返回值是undefined
 (3)同步执行then1then2这个.then函数，因为上面一个.then回调函数体还没有执行，返回一个pending状态的Promise，所以这个.then函数回调函数体放在上面的Promise对象的队列中
 (4)最后，这个函数没有返回任何东西，所以根据https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Promise/then，这个Promise的状态变成了fulfilled，即第一个.then函数返回的Promise对象的状态改变了。这个时候其实是执行了resolve函数切换状态，这个函数里面会看这个Promise对象自己的队列里面是不是有回调函数，然后把这些放到timeTick队列里面。所以第二个.then函数体放到了队列里面。注意这个时候then1then1和then2的函数体都在tick队列中，但是都没有执行。再加上js是单线程，会顺序执行tick队列里面的回调函数体，所以接下来就执行then1then1和then2
 7.分别执行then1then1和then2两回调函数体，所以会先打印then1 then1，然后是'then2'
 8.then1then1回调函数体执行完毕的时候，她的Promise对象也会切换为fulfilled，并且把then1then2回调函数体放在tick队列中
 9.最后执行then1then2函数体，打印'then1 then2'。完美收官
 */
 
 // 参考：https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Promise/then
 function testThenOrder1(){
	 // using a resolved promise, the 'then' block will be triggered instantly, 
	// but its handlers will be triggered asynchronously as demonstrated by the console.logs
	const resolvedProm = Promise.resolve(33);

	then1 = resolvedProm.then(value => {
		console.log("[log]----this gets called after the end of the main stack. the value received and returned is: " + value);
		
		then1promise = new Promise((resolve, reject) => {
            console.log("[log]----then 1 promise")
            resolve('then1promise')
        })
		console.log('then1promise =', then1promise)
		
		then1then1 = then1promise.then(() => {
			console.log("[log]----then1 then1")
			return 'then1then1'
		})
		console.log('then1then1 = ', then1then1)
		
		then1then2 = then1then1.then(() => {
			console.log("[log]----then1 then2")
			return 'then1then2'
		})
		
		console.log('then1then2 = ', then1then2)
		
		// return then1promise;
		return 1;
	});
	// instantly logging the value of thenProm
	console.log("then1 = ", then1);
	
	then2 = then1.then( value => {
		console.log("[log]----then2 = " + value);
		return 2;
		
	});
	// instantly logging the value of thenProm
	console.log('then2 = ', then2);

	// using setTimeout we can postpone the execution of a function to the moment the stack is empty
	setTimeout(() => {
		console.log('waiting.........................')
		console.log("then1 = ", then1);
		console.log("then2 = ", then2);
		console.log('then1promise = ', then1promise);
		console.log('then1then1 = ', then1then1);
		console.log('then1then1 = ', then1then2);
	});

	// setTimeout(() => {
		// console.log('waiting 1 second.....................')
		// console.log("then1 = ", then1);
		// console.log("then2 =", then2);
		// console.log('then1promise = ', then1promise);
		// console.log('then1then1 = ', then1then1);
		// console.log('then1then2 = ', then1then2);
	// }, 1000);

	// 上面的代码会依次返回：
	// Promise {[[PromiseStatus]]: "pending", [[PromiseValue]]: undefined}
	// "this gets called after the end of the main stack. the value received and returned is: 33"
	// Promise {[[PromiseStatus]]: "resolved", [[PromiseValue]]: 33}
 }
 // testThenOrder1();
 
 // then里面return的Promise代替默认的promise
 function testThenOrder2(){
	Promise.resolve("foo")
	// 1. 接收 "foo" 并与 "bar" 拼接，并将其结果做为下一个 resolve 返回。
	.then(function(string) {
		// 情况1，return一个promise
		// return new Promise(function(resolve, reject) {
			
		// 情况2：直接new一个promise，没有return
		// new Promise(function(resolve, reject) {
			// setTimeout(function() {
				// string += 'bar';
				// resolve(string);
			// }, 1000);
		// });
		
		// 情况3：直接new一个promise，没有return，但是有then
		new Promise(function(resolve, reject) {
			setTimeout(function() {
				string += 'bar';
				resolve(string);
			}, 2000);
		})
		.then(string=>{
			console.log('111 ', string);
		});
	})
	// 2. 接收 "foobar", 放入一个异步函数中处理该字符串
	// 并将其打印到控制台中, 但是不将处理后的字符串返回到下一个。
	.then(function(string) {
		setTimeout(function() {
			string += 'baz';
			console.log(string);
		}, 1000)
		return string;
	})
	// 3. 打印本节中代码将如何运行的帮助消息，
	// 字符串实际上是由上一个回调函数之前的那块异步代码处理的。
	.then(function(string) {
		console.log("Last Then:  oops... didn't bother to instantiate and return " +
					"a promise in the prior then so the sequence may be a bit " +
					"surprising");

		// 注意 `string` 这时不会存在 'baz'。
		// 因为这是发生在我们通过setTimeout模拟的异步函数中。
		console.log(string);
	});

	// logs, in order:
	// Last Then: oops... didn't bother to instantiate and return a promise in the prior then so the sequence may be a bit surprising
	// foobar
	// foobarbaz
 }
 // testThenOrder2();
 
 
 //////////////////////////////////////////////
 // catch
 function catch0(){
	 var p1 = new Promise(function(resolve, reject) {
	  resolve('Success');
	});

	p1.then(function(value) {
	  console.log(value); // "Success!"
	  throw 'oh, no!';
	}).catch(function(e) {
	  console.log(e); // "oh, no!"
	}).then(function(){
	  console.log('after a catch the chain is restored');
	}, function () {
	  console.log('Not fired due to the catch');
	});

	// 以下行为与上述相同
	// p1.then(function(value) {
	  // console.log(value); // "Success!"
	  // return Promise.reject('oh, no!');
	// }).catch(function(e) {
	  // console.log(e); // "oh, no!"
	// }).then(function(){
	  // console.log('after a catch the chain is restored');
	// }, function () {
	  // console.log('Not fired due to the catch');
	// });
 }
 // catch0();
 
 function catch2(){
	 //创建一个新的 Promise ，且已决议
	var p1 = Promise.resolve("calling next");

	var p2 = p1.catch(function (reason) {
		//这个方法永远不会调用
		console.log("catch p1!");
		console.log(reason);
	});

	p2.then(function (value) {
		console.log("next promise's onFulfilled"); /* next promise's onFulfilled */
		console.log(value); /* calling next */
	}, function (reason) {
		console.log("next promise's onRejected");
		console.log(reason);
	});
}
// catch2();


function catch5(num){
	var p1 = new Promise(function(resolve, reject) {
		if (num == 0){
			reject('failed')
		}
		else{
			resolve('Success');
		}
	});
	
	p1.then(function(value) {
	  console.log(value); // "Success!"
	  return Promise.resolve(value);
	}).catch(function(e) {
		console.log(e); // "oh, no!"
		return Promise.reject('oh, no!');
	}).then(function(){
		console.log('after a catch the chain is restored');
	}, function () {
		console.log('Not fired due to the catch');
	});
}
// catch5();


/////////////////////////////////////////////////////////////////
// promise all/reduce
function all0(){
	Promise.all(
	[
		Promise.reject({ code: 500, msg: "error" }),
		Promise.resolve({ code: 200, list: [] }),
		Promise.resolve({ code: 200, list: [] })
	].map(p => p.catch(e => e))
	)
	.then(res => {
		console.log("res=>", res);
	})
	.catch(error => {
		console.log("error=>", error);
	});
}
all0();