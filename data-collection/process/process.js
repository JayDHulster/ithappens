const redis = require('redis');
const {InfluxDB, Point} = require('@influxdata/influxdb-client')
require('dotenv').config()

console.log("SCRIPT STARTED");

const url = 'http://influxdb:8086'
// const token = process.env.INFLUX_TOKEN
const token = 'LOjUMPGehdZ3SIFy01jpNH-pdGyFpuEYWDDdZeFv5MV3h3fEvNy5DFiC7VqCOTGvhZ4Hw2txrsZATLRcKBPo9A=='
const client = new InfluxDB({url, token})

let org = `vives`
let bucket = `biosensor`

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

  const stringdata = await redisClient.BLPOP('pressure', 0);
  const data = JSON.parse(stringdata.element)

try {
let point = new Point('pressure')
.timestamp( new Date(data.timestamp))
.floatField('value', data.value)
.tag('device', 'biosensor')
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