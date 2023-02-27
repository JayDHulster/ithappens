const redis = require('redis');
const {InfluxDB, Point} = require('@influxdata/influxdb-client')
require('dotenv').config()

console.log("SCRIPT STARTED");

const url = 'http://influxdb:8086'
const token = process.env.INFLUX_TOKEN
const client = new InfluxDB({url, token})

let org = `vives`
let bucket = `dust`

let writeApi = client.getWriteApi(org, bucket, 'ns')

const redisClient = redis.createClient({
    socket: {
        host: 'redis',
        port: 6379
    },
    password: ''
  });

(async () => {
await redisClient.connect();


while(1) {

  const stringdata = await redisClient.BLPOP('dust', 0);
  const data = JSON.parse(stringdata.element)

try {
let point = new Point('dust')
.timestamp( new Date(data.received_at))
.intField('moisture_lv1', data.uplink_message.decoded_payload.moistureLevel_1)
.intField('moisture_lv2', data.uplink_message.decoded_payload.moistureLevel_2)
.intField('moisture_lv3', data.uplink_message.decoded_payload.moistureLevel_3)
.intField('moisture_lv4', data.uplink_message.decoded_payload.moistureLevel_4)
.floatField('battery_voltage', data.uplink_message.decoded_payload.batteryVoltage)
.tag('device_id', data.end_device_ids.device_id)
writeApi.writePoint(point)
console.log(point)
} catch (error) {
  console.log(error)
}

}
})();

redisClient.on('ready', () => {
console.log("REDIS CONNECTED");
});

redisClient.on('error', err => {
console.log('Error ' + err);
});