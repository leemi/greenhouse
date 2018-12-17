/*
A specificity of indoor plant growing with grow lights is that you need to figure out what
the light cycle for the plants will be, as there won’t be sun that dictates when plants
get the light. Light cycles are used to simulate the conditions of day and night for indoor
gardens and you need to know your light cycles to grow your succulents faster.
A general rule of thumb for growing any plants under lights is that the more light you give
them, the faster they will grow and the same can be attributed to succulents. If you are
overwintering the plants, then I would put the succulents at first on 20/4 light cycle
meaning that I would leave the lights on for 20 hours a day and then turn them off for
4 hours and slowly increase the darkness period so the light cycle is 16/8 (light/dark).
Succulents need to know when it is winter, so they can start their dormant process.
If you use grow lights all year around for your succulents, during the summer you can
have them on 24/0 or 20/4 light cycle for them to grow quickly and thrive.
 */

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
var readline = require('readline');
var schedule = require('node-schedule');

const port = new SerialPort("COM5", {
  baudRate: 9600,
});
const parser = port.pipe(new Readline({ delimiter: '\r\n' }))

parser.on('data', function(data) {
  console.log('<', data);
});

schedule.scheduleJob('0 8 * * *', function() {
  port.write('lights on');
});

schedule.scheduleJob('0 0 * * *', function() {
  port.write('lights off');
});

var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

rl.on('line', function(line){
  console.log('>', line);
  port.write(line);
});

setInterval(() => {}, 1 << 30);
