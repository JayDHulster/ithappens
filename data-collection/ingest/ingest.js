require('dotenv').config()

const mqtt = require('mqtt')
const redis = require('redis');
console.log("SCRIPT STARTED");

const redisClient = redis.createClient({
  socket: {
      host: 'redis', 
      port: 6379
  },
  password: ''
});

const host = process.env.DUST_HOST
const port = process.env.DUST_PORT

const connectUrl = `mqtts://${host}:${port}`
const client = mqtt.connect(connectUrl, {
  username: process.env.DUST_USER,
  password: process.env.DUST_PASSWORD,

})

const device_1 = process.env.DEVICE_1
const device_2 = process.env.DEVICE_2
const device_3 = process.env.DEVICE_3
const device_4 = process.env.DEVICE_4
const device_5 = process.env.DEVICE_5

client.on('connect', function () {
    console.log("MQTTS CONNECTED")
    client.subscribe(device_1)
    client.subscribe(device_2)
    client.subscribe(device_3)
    client.subscribe(device_4)
    client.subscribe(device_5)

});

(async () => {
  await redisClient.connect();
})();

redisClient.on('ready', () => {
  console.log("REDIS CONNECTED");
});

redisClient.on('error', err => {
  console.log('Error ' + err);
});

client.on('message', function (topic, message) {
  // message is Buffer
  redisClient.LPUSH('dust', message);

  console.log(message.toString() + " AT " + Date.now().toString())
  // client.end()
})