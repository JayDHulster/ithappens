# import redis
from datetime import datetime
import pytz

# try:
#     r = redis.Redis(host='localhost', port=6379, db=0)
#     r.lpush('test', '{ "timestamp": "' + time.strftime("%d-%m-%Y %H:%M:%S") + '", "value": "' + '123' + '" }')
# except Exception as e:
#     print(e)
print(datetime.now(pytz.timezone('Europe/Brussels')).isoformat())