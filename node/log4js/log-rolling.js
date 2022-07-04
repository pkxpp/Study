const log4js = require('log4js');

log4js.configure({
  appenders: {
    console: {
      type: 'console'
    },
    file: {
      type: 'file',
      filename: 'tmp-test.log',
      maxLogSize: 1024 * 1024,
      backups: 3
    },
	dateFile: {
      type: 'dateFile',
      filename: 'tmp-test.log',
      maxLogSize: 1024 * 1024,
      backups: 3
    }
  },
  categories: {
	  test: { appenders: ['console'], level: 'info' },
	  test1: { appenders: ['console', 'dateFile'], level: 'info' },
    default: { appenders: ['console', 'file'], level: 'info' }
	
  }
});
const log = log4js.getLogger('test1');

function doTheLogging(x) {
  log.info('Logging something %d', x);
}
let i = 0;
for (; i < 50; i += 1) {
  doTheLogging(i);
}