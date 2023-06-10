from datetime import datetime
import pytz
import paho.mqtt.client as mqtt
import redis

# create a redis client instance
r = redis.Redis(host='redis', port=6379, db=0)
print('Connection set up to Redis!')

# MQTT broker information
broker_address = "mqtt.devbit.be"
broker_port = 1883

# Topic to subscribe to
topic = "biosensor/id_123/pressure"

# Callback function for MQTT connection
def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")
    client.subscribe(topic)


# Define callback functions which will be called when certain events happen.
def connected(client):
    # Connected function will be called when the client is connected to Adafruit IO.
    # This is a good place to subscribe to feed changes.  The client parameter
    # passed to this function is the Adafruit IO MQTT client so you can make
    # calls against it easily.
    print('Connected to Adafruit IO!  Listening for {0} changes...'.format(ADAFRUIT_FEED))
    # Subscribe to changes on a feed named DemoFeed.
    client.subscribe(ADAFRUIT_FEED)

def subscribe(client, userdata, mid, granted_qos):
    # This method is called when the client subscribes to a new feed.
    print('Subscribed to {0} with QoS {1}'.format(ADAFRUIT_FEED, granted_qos[0]))

def disconnected(client):
    # Disconnected function will be called when the client disconnects.
    print('Disconnected from Adafruit IO!')
    sys.exit(1)

def message(client, feed_id, payload):
    # Message function will be called when a subscribed feed has a new value.
    # The feed_id parameter identifies the feed, and the payload parameter has
    # the new value.
    payload = float(payload) / 100
    print('Feed {0} received new value: {1}'.format(feed_id, payload))
    try:
        r.lpush('pressure', '{ "timestamp": "' + datetime.now(pytz.timezone('Europe/Brussels')).isoformat() + '", "value": "' + str(payload) + '" }')
    except Exception as e:
        print(e)
        sys.exit()


# Create an MQTT client instance.
client = MQTTClient(ADAFRUIT_USER, ADAFRUIT_IO_KEY)

# Setup the callback functions defined above.
client.on_connect    = connected
client.on_disconnect = disconnected
client.on_message    = message
client.on_subscribe  = subscribe

# Connect to the Adafruit IO server.
client.connect()

# Start a message loop that blocks forever waiting for MQTT messages to be
# received.  Note there are other options for running the event loop like doing
# so in a background thread--see the mqtt_client.py example to learn more.
client.loop_blocking()