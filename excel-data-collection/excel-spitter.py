import csv
import paho.mqtt.client as mqtt
import datetime
import pytz

# MQTT broker information
broker_address = "mqtt.devbit.be"
broker_port = 1883

# Topic to subscribe to
topic = "biosensor/id_123/pressure"

# CSV file to write the data
csv_file = "data.csv"

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

    # Convert float value to string representation with comma as decimal separator
    data = data.replace('.', ',')

    # Write data and timestamp to CSV file
    with open(csv_file, "a", newline="") as file:
        writer = csv.writer(file, delimiter=";")
        writer.writerow([timestamp, data])

        print(f"Data received: {data}. Timestamp: {timestamp}. Written to {csv_file}")

# Create MQTT client
client = mqtt.Client()

# Assign callback functions
client.on_connect = on_connect
client.on_message = on_message

# Connect to MQTT broker
client.connect(broker_address, broker_port)

# Start the MQTT loop
client.loop_forever()
