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
const readline = require('readline');
const schedule = require('node-schedule');
const SunCalc = require('suncalc');
const delay = require('delay');

const LAT_LNG = [43.653225, -79.383186];

const port = new SerialPort("COM5", {
  baudRate: 9600,
});
const parser = port.pipe(new Readline({ delimiter: '\r\n' }))

port.on('error', function(err) {
  console.log('Error: ', err.message)
});

port.on('open', function() {
  console.log('Connection Opened');
});

parser.on('data', function(data) {
  console.log('<', data);
});

schedule.scheduleJob('0 8 * * *', function() {
  port.write('lights on');
});

schedule.scheduleJob('0 0 * * *', function() {
  port.write('lights off');
});

let rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

rl.on('line', function(line){
  if (line === "play") {
    playDay();
  } else {
    console.log('>', line);
    port.write(line);
  }
});

function playDay() {

  (async() => {

    let start = new Date().setHours(0, 0, 0, 0);
    let end = new Date();
    end.setHours(0, 0, 0, 0);
    end.setHours(24);
    let cur = new Date(start);

    const FRAME_DUR = 1000;
    const ANIMATION_LENGTH = 60000; // 10 s
    const NUM_FRAMES = ANIMATION_LENGTH / FRAME_DUR;
    const MS_IN_DAY = 86400000;
    const STEP_SIZE = MS_IN_DAY / NUM_FRAMES;

    while (cur < end) {

      console.log(cur.toString());

      let pos = SunCalc.getPosition(cur, LAT_LNG[0], LAT_LNG[1]);

      setColorAngle(pos.altitude * 180/Math.PI);

      cur = new Date(cur.getTime() + STEP_SIZE);

      await delay(FRAME_DUR);
    }

  })();

}

function setColorAngle(angle) {
  // let color = [0, 0, 0, 0];

  // Nighttime < -18
  // color = [0, 0, 0, 0];
  // // Astronomical -15
  // color = [0, 42, 114, 128];
  // // Nautical -9
  // color = [0, 98, 161, 128];
  // // Civil -3
  // color = [85, 160, 217, 128];
  // // Golden 1
  // color = [233, 180, 47, 192];
  // // Daytime > 6
  // color = [255, 255, 255, 255];

  const x = angle;
  r = 152.3911 + 18.55365*x + 0.5380059*x^2;
  g = 175.7893 + 8.601756*x + 0.4053063*x^2 + 0.04862254*x^3 + 0.001265863*x^4
  b = 98.20324 - 51.66196*x - 1.053027*x^2 + 1.369691*x^3 + 0.1382349*x^4 + 0.003815818*x^5;
  br = 155.8132 + 7.02256*x + 1.361226*x^2 + 0.07769448*x^3;

  r = Math.max(0, Math.min(r, 255));
  g = Math.max(0, Math.min(g, 255));
  b = Math.max(0, Math.min(b, 255));
  br = Math.max(0, Math.min(br, 255));

  console.log(`lights color ${ r } ${ g } ${ b } ${ br }`);

  port.write(`lights color ${ r } ${ g } ${ b }`);
  port.write(`lights brightness ${ br }`);
}

setInterval(() => {}, 1 << 30);
