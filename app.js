const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

var schedule = require('node-schedule');

const port = new SerialPort("COM5", {
  baudRate: 9600,
});
const parser = port.pipe(new Readline({ delimiter: '\r\n' }))

parser.on('data', function(data) {
  console.log('<', data);
});

schedule.scheduleJob('0 14 * * *', function() {
  console.log('> lights on');
  port.write('lights on');
});

schedule.scheduleJob('0 10 * * *', function() {
  console.log('> lights off');
  port.write('lights off');
});

setInterval(() => {}, 1 << 30);
