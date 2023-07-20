import paho.mqtt.client as mqtt
import json
import mysql.connector
import time
from datetime import datetime

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  password="PASSWORD",
  database="mydatabase"
)

mycursor = mydb.cursor()


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("connected with result code " + str(rc))
    client.subscribe("CO_LVL")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))
    data = json.loads(msg.payload)
    sensor = data["sensor"]
    co_level = data["CO level"]
    now = datetime.now()
    timestamp = now.strftime("%d/%m/%Y %H:%M:%S")
    if sensor == 2:

      if co_level > 100 and co_level <= 300:
          client.publish("client2/consistency", payload="moderate")
      elif co_level > 300:
          client.publish("client2/consistency", payload="high")
    

    elif sensor == 3:

      if co_level > 100 and co_level <= 300:
          client.publish("client3/consistency", payload="moderate")
      elif co_level > 300:
          client.publish("client3/consistency", payload="high")


    elif sensor == 4:

      if co_level > 100 and co_level <= 300:
          client.publish("client4/consistency", payload="moderate")
      elif co_level > 300:
          client.publish("client4/consistency", payload="high")


    mycursor.execute("INSERT INTO CO_MONITORING (`sensor`, `co_level`, `timestamp`) VALUES (%s, %s, %s)", (sensor, co_level, timestamp))
    mydb.commit()
    print(mycursor.rowcount, "record inserted.")

    
client2 = mqtt.Client(client_id="fd00::202:2:2:2")	#linked to unit A
client2.on_connect = on_connect
client2.on_message = on_message
client2.connect("127.0.0.1", 1883, 60)

client3 = mqtt.Client(client_id="fd00::203:3:3:3")	#linked to unit B
client3.on_connect = on_connect
client3.on_message = on_message
client3.connect("127.0.0.1", 1883, 60)

client4 = mqtt.Client(client_id="fd00::204:4:4:4")	#linked to unit C
client4.on_connect = on_connect
client4.on_message = on_message
client4.connect("127.0.0.1", 1883, 60)




client2.loop_forever()
client3.loop_forever()
client4.loop_forever()