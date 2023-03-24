import os
from dotenv import load_dotenv
# pip install python-dotenv

load_dotenv()

# Import library and create instance of REST client.
from Adafruit_IO import Client
aio = Client(os.getenv('ADAFRUIT_USER'), os.getenv('ADAFRUIT_IO_KEY'))

# Send the value 100 to a feed called 'Foo'.
# aio.send('Foo', 100)

# Retrieve the most recent value from the feed 'Foo'.
# Access the value by reading the `value` property on the returned Data object.
# Note that all values retrieved from IO are strings so you might need to convert
# them to an int or numeric type if you expect a number.
data = aio.receive(os.getenv('ADAFRUIT_FEED'))
print('Received value: {0}'.format(data.value))