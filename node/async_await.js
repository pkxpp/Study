
async function test(ctx,next){
    doThing().then(console.log('done Thing'))
    doAnotherThing();
    console.log('this way');
}

  async function doThing() {
    // doA();
    // doB();
	
	await doA();
    await doB();
  }
  
  function doAnotherThing() {
    console.log('do another thing')
  }
  
  async function doA() {
    return new Promise(resove => {
      setTimeout(() => {
        console.log('done A')
        resove()
      }, 1000)
    })
  }
  
  async function doB() {
    return new Promise(resove => {
      setTimeout(() => {
        console.log('done B')
        resove()
      }, 100)
    })
  }
  
  // test()
  
async function test1(){
	var a = await 3;
	return a;
}
// console.log(test1())
// console.log(test1().then((resolve, reject)=>{
	// console.log('resolve:', resolve)
// }))

  
///////////////////////////////////////////////////////////////
// wait ms milliseconds
function wait(ms) {
  return new Promise(r => setTimeout(r, ms));
}

async function hello() {
  await wait(500);
  return 'world';
}

// console.log(111)
// a = hello().then(resolve=>{
	// console.log(333, resolve)
// })
// a = Promise.all([(async()=>console.log(await hello()))()])
// console.log(222)
// console.log(a)

///////////////////////////////////////////////////////////////
// reference: https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Statements/async_function
var resolveAfter2Seconds = function() {
  console.log("starting slow promise");
  return new Promise(resolve => {
    setTimeout(function() {
      resolve("slow");
      console.log("slow promise is done");
    }, 2000);
  });
};

var resolveAfter1Second = function() {
  console.log("starting fast promise");
  return new Promise(resolve => {
    setTimeout(function() {
      resolve("fast");
	  // Promise.reject("fast")
      console.log("fast promise is done");
    }, 1000);
  });
};

var sequentialStart = async function() {
  console.log('==SEQUENTIAL START==');

  // 1. Execution gets here almost instantly
  const slow = await resolveAfter2Seconds();
  console.log(slow); // 2. this runs 2 seconds after 1.

  const fast = await resolveAfter1Second();
  console.log(fast); // 3. this runs 3 seconds after 1.
}

var concurrentStart = async function() {
  console.log('==CONCURRENT START with await==');
  const slow = resolveAfter2Seconds(); // starts timer immediately
  const fast = resolveAfter1Second(); // starts timer immediately
  // console.log(111, slow, fast)
  // 1. Execution gets here almost instantly
  console.log(await slow); // 2. this runs 2 seconds after 1. 1秒后这个立就执行了，但是没有打印fast，这个估计就是异步消息队列的原因吧？前面一个要等2秒还没处理完
  // 1秒和2秒不是很明显，改成1秒和3秒就明显多了，fast promise is done打印之后等一会才打印后面的
  // 因为是await，所以保证顺序执行，所以最后的fast才打印
  console.log(await fast); // 3. this runs 2 seconds after 1., immediately after 2., since fast is already resolved
  // 总结：
  // 1.两个定时器同时运行，所以总共只需要2秒
  // 2.await保证顺序执行，是说返回的结果顺序？但是执行顺序并不保证？因为fast promise is done还是先打印的。看第3条再来回答这个问题的话就是，resolveAfter2Seconds和resolveAfter1Second同时运行，而他们本身并不会因为await阻塞，所以fast promise is done会先执行完，所以先打印。
  // 3.await 表达式会暂停当前 async function 的执行，等待 Promise 处理完成。所以第一个slow是一个Promise，需要2秒种的执行时间，即使第二个Promise(fast)已经在1秒钟的时候执行完了，但是由于async函数暂停了，所以需要第二个console.log才执行，跟消息队列没有关系。
  // 无论是否使用 await，异步函数都会返回 Promise. 这一条保证了concurrentStart返回是一个Promise，如果我在后面加了一个return的话
  console.log('end');
  return slow
}

var concurrentPromise = function() {
  console.log('==CONCURRENT START with Promise.all==');
  return Promise.all([resolveAfter2Seconds(), resolveAfter1Second()]).then((messages) => {
    console.log(messages[0]); // slow
    console.log(messages[1]); // fast
  });
}

var parallel = async function() {
  console.log('==PARALLEL with await Promise.all==');
  
  // Start 2 "jobs" in parallel and wait for both of them to complete
  await Promise.all([
		// 箭头函数：返回console.log的结果
		// 里面我是await resolveAfter2Seconds函数
      (async()=>console.log(await resolveAfter2Seconds()))(),
      (async()=>console.log(await resolveAfter1Second()))()
  ]);
  // 总结：
  // 1.
}

// This function does not handle errors. See warning below!
var parallelPromise = function() {
  console.log('==PARALLEL with Promise.then==');
  resolveAfter2Seconds().then((message)=>console.log(message));
  resolveAfter1Second().then((message)=>console.log(message));
}

// sequentialStart(); // after 2 seconds, logs "slow", then after 1 more second, "fast"

// wait above to finish
// setTimeout(concurrentStart, 4000); // after 2 seconds, logs "slow" and then "fast"
// console.log(concurrentStart())
// Promise.all([(async()=>console.log(await concurrentStart()))()])

// wait again
// setTimeout(concurrentPromise, 7000); // same as concurrentStart

// wait again
// setTimeout(parallel, 10000); // truly parallel: after 1 second, logs "fast", then after 1 more second, "slow"
// setTimeout(parallel, 1000); // truly parallel: after 1 second, logs "fast", then after 1 more second, "slow"

// wait again
// setTimeout(parallelPromise, 13000); // same as parallel
// setTimeout(parallelPromise, 1000); // same as parallel


//////////////////////////////////////////////////////
// await: https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Operators/await
function resolveAfter2Seconds1(x) {
  return new Promise(resolve => {
    setTimeout(() => {
      resolve(x);
	  console.log('resolveAfter2Seconds1 done.')
    }, 2000);
  });
}

// 如果一个 Promise 被传递给一个 await 操作符，await 将等待 Promise 正常处理完成并返回其处理结果。
async function f1() {
  var x = await resolveAfter2Seconds1(10);
  console.log(x); // 10
}
// f1();

// 如果该值不是一个 Promise，await 会把该值转换为已正常处理的Promise，然后等待其处理结果。
async function f2() {
  var y = await 20;
  console.log(y); // 20
}
// f2();

// 如果 Promise 处理异常，则异常值被抛出。
async function f3() {
  try {
    var z = await Promise.reject(30);
  } catch (e) {
    console.log(e); // 30
  }
}
// f3();

//////////////////////////////////////////
var resolveAfterNSecond = function(time) {
  console.log("starting promise, wait: " + time);
  return new Promise((resolve, reject) => {
    setTimeout(function() {
		if(time%2000 === 0){
			resolve(time);
		}
		else{
			reject('error')
		}
      
	  // Promise.reject("fast")
      console.log("promise is done, time = ", time);
    }, time);
  });
};

function forPromiseAll(n){
	var l = [];
	for(var i = 0; i < n; ++i){
		var r = resolveAfterNSecond((i+1) * 1000);
		l.push(r);
	}
	Promise.all(l)
	.then((ret)=>{
		console.log(ret);
	});
}
forPromiseAll(2);

async function forAsyncAwait(n){
	for(var i = 0; i < n; ++i){
		await resolveAfterNSecond((i+1) * 1000);
	}
	
	console.log('end');
}
// forAsyncAwait(3);