var events = require('events');
var util = require('util');
var a = function () {
    // events.EventEmitter.call(this);
};
util.inherits(a, events.EventEmitter);
console.log(a, new a())

// events.EventEmitter.call(this);注释之前输出结果
// F:\study\node\code>node events_study.js
// [Function: a] a {
  // _events: [Object: null prototype] {},
  // _eventsCount: 0,
  // _maxListeners: undefined
// }

// events.EventEmitter.call(this);注释之后输出结果
// [Function: a] a {}