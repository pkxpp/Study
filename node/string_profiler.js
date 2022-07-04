var str = {"packageID":12,"area":"1024","syncFuncName":"NewRemotePlayer","newPlayerInfo":{"uuid":"27b5c345-12a5-4ce1-bec5-144f0c9ec241","playerID":68,"area":"1024","userName":"9186cef5-8904-4847-8737-ecb2a470dcbd","templateID":1,"actorPath":"UGCSource\\player\\F1_Model.actor","pos":{"X":3854.8707335249765,"Y":0,"Z":3905.8043766931773,"W":128}},"ignoreId":68}

var POS_TEMP = {
	type: 'object',
	properties: {
		X: {
			type: 'number'
		},
		Y: {
			type: 'number'
		},
		Z: {
			type: 'number'
		},
	}
}

var IF_TEMP = { 
	'if': {
		'type': 'string'
	},
	'then': {
		'type': 'string'
	},
	 'else': {
		'type': 'integer'
	 }
}

const fastJson = require('fast-json-stringify')
const stringify = fastJson({
  // title: 'Example Schema',
  type: 'object',
  properties: {
    firstName: {
      type: 'string'
    },
    lastName: {
      type: 'string'
    },
    age: {
      description: 'Age in years',
      type: 'integer'
    },
    reg: {
      type: 'string'
    },
	pos: POS_TEMP,
	testArray: {
		type: 'array',
		items: POS_TEMP
	}
  }
})

console.log(stringify({
	test: 'test',
  firstName: 'Matteo',
  lastName: 0,
  age: 'test',
  reg: /"([^"]|\\")*"/,
  pos:{X: 1, Y: 2, Z: 3},
  parse: 1,
  testArray: [{X: 4, Y: 5, Z: 6}, {X: 7, Y: 8, Z: 9}],
}))
