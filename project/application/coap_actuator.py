import socket
import sys
import time
from coapthon.client.helperclient import HelperClient
from coapthon.utils import parse_uri
import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  password="PASSWORD",
  database="mydatabase"
)

mycursor = mydb.cursor()


port = 5683
factory = "/factory"
res_led = "/led"

host2 = "fd00::205:5:5:5"	#linked to unit A
client2 = HelperClient(server=(host2, port))
response2 = client2.get(factory)
print("Client 2 answer:", response2.pretty_print())
sensor2 = 28;

host3 = "fd00::206:6:6:6"	#linked to unit B
client3 = HelperClient(server=(host3, port))
response3 = client3.get(factory)
print("Client 3 answer:", response3.pretty_print())
sensor3 = 28;

host4 = "fd00::207:7:7:7"	#linked to unit C
client4 = HelperClient(server=(host4, port))
response4 = client4.get(factory)
print("Client 4 answer:", response4.pretty_print())
sensor4 = 28;





while True:

    mydb.close()

    mydb = mysql.connector.connect(
      host="localhost",
      user="root",
      password="PASSWORD",
      database="mydatabase"
    )


    mycursor = mydb.cursor()

    # Actuator 2:
    #update sensor2
    mycursor.execute("SELECT co_level FROM CO_MONITORING WHERE sensor = 2")
    result2 = mycursor.fetchall()
    sensor2 = result2[-1][-1]
    print("CO level in Unit A is: %d ppm" % sensor2)

    if sensor2 < 100:
        client2.post(res_led, payload="mode=low")
    elif sensor2 > 100 and sensor2 <= 300:
        client2.post(res_led, payload="mode=moderate")
    elif sensor2 > 300:
        client2.post(res_led, payload="mode=high")

    client2.put(factory, payload="name=unit_A&value={}".format(sensor2))

    

    # Actuator 3
    #update sensor3
    mycursor.execute("SELECT co_level FROM CO_MONITORING WHERE sensor = 3")
    result3 = mycursor.fetchall()
    sensor3 = result3[-1][-1]
    print("CO level in Unit B is: %d ppm" % sensor3)

    if sensor3 < 100:
        client3.post(res_led, payload="mode=low")
    elif sensor3 > 100 and sensor3 <= 300:
        client3.post(res_led, payload="mode=moderate")
    elif sensor3 > 300:
        client3.post(res_led, payload="mode=high")

    client3.put(factory, payload="name=unit_B&value={}".format(sensor3))
 



    # Actuator 4
    #update sensor4
    mycursor.execute("SELECT co_level FROM CO_MONITORING WHERE sensor = 4")
    result4 = mycursor.fetchall()
    sensor4 = result4[-1][-1]
    print("CO level in Unit C is: %d ppm" % sensor4)

    if sensor4 < 100:
        client4.post(res_led, payload="mode=low")
    elif sensor4 > 100 and sensor4 <= 300:
        client4.post(res_led, payload="mode=moderate")
    elif sensor4 > 300:
        client4.post(res_led, payload="mode=high")

    client4.put(factory, payload="name=unit_C&value={}".format(sensor4))
 

    time.sleep(31)


client2.stop()
client3.stop()
client4.stop()
