var async = require('async');
var a = 10;
async.waterfall([
    function(cb) {
        console.log("getb")
        setTimeout(function() {
            if (a == 0) {
                cb(new Error("a不能为0"));
            } else {
                var b = 1 / a;
                cb(null, b); //在这里通过回调函数把b传给下一个函数，记得一定要加上null，才能调用数组中得下一个函数，否则，会直接调用最终的回调函数，然后结束函数，则后面的函数将不再执行
                //如果这里写成cb(b);
                //结果会变成：
                /**
                 *getb
                 *0.1
                 **/
            }
        }, 1000);
    },
    function(b, cb) {
        setTimeout(function() {
            console.log("getc")
            var c = b + 1;
            cb(null,c);
        }, 1000);
    }
], function(err, result) {
    if (err) {
        console.log(err);
    } else {
        console.log('c:' + result)
    }
});