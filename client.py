import serial
import time
import json
import sys
import urllib.request as urllib_request
who = sys.argv[1]
num = sys.argv[2]
handle = urllib_request.urlopen("https://api.twitter.com/1/statuses/user_timeline/" + who + ".json?count=" + num + "&include_rts=1")
data = handle.read()
tweets = json.loads(data.decode('utf8'))
for tweet in tweets:
    text = tweet['text']
    print(text)
    ser = serial.Serial('/dev/ttyACM0', 1200)
    time.sleep(2)
    i = 0
    for c in text:
        ser.write(bytes('' + c, 'ISO8859-1'))
        if i % 15 == 0:
            time.sleep(1)
        else:
            time.sleep(0.003)
        i += 1
    ser.write(bytes('\n', 'ISO8859-1'))
    ser.write(bytes('\n', 'ISO8859-1'))
