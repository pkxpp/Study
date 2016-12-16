/******************************************************
* file    : 简单记录
* author  : page(pk)
* time    : 2016/11/15
*******************************************************/
var template = {
    // data = [];
};

function compute(data)
{
    if ( !data instanceof Array) return;

    var sum = 0;
    for (var i = 0; i < data.length; ++i){
        sum += data[i];
    }
    console.log("sum = " + sum);
}

var a10 = [5149.87, -2278.10, 12297.36, 1008.57, 270, 469, 273, 5328];
compute(a10);

var a11 = [9798.79, -2842.81, 8816.51, 1199.65, 220, 282.73, 273.95, 5359];
compute(a11);

var a12 = [11744.38, -6727.77, 5329.82, 1390.73, 14.5, 231.47, 269.95, 7754.967];
compute(a12);
