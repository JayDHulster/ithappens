import csv
import paho.mqtt.client as mqtt
import datetime
import sys
import pytz
import redis

# MQTT broker information
broker_address = "mqtt.devbit.be"
broker_port = 1883

# Topic to subscribe to
topic = "biosensor/id_123/pressure"

# create a redis client instance
r = redis.Redis(host='redis', port=6379, db=0)
print('Connection set up to Redis!')

# Callback function for MQTT connection
def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")
    client.subscribe(topic)

# Callback function for MQTT message
def on_message(client, userdata, msg):
    # Decode the received message
    data = msg.payload.decode("utf-8")

    # Get current timestamp with the specified timezone
    timestamp = datetime.datetime.now(pytz.timezone('Europe/Brussels')).isoformat()
    try:
        r.lpush('pressure', '{ "timestamp": "' + timestamp + '", "value": "' + str(data) + '" }')
        print(f"Data received: {data}. Timestamp: {timestamp}. Written to redis!")
    except Exception as e:
        print(e)
        sys.exit()

# Create MQTT client
client = mqtt.Client()

# Assign callback functions
client.on_connect = on_connect
client.on_message = on_message

# Connect to MQTT broker
client.connect(broker_address, broker_port)

# Start the MQTT loop
client.loop_forever()
