import paho.mqtt.client as mqttClient
import paho.mqtt.publish as publish
from collections import OrderedDict
import time, json, os
host = "172.20.10.5"
def on_connect(client, userdata, flags, rc):

    if rc == 0:

        print("Connected to broker")

        global Connected                #Use global variable
        Connected = True                #Signal connection

    else:

        print("Connection failed")

def on_message(client, userdata, message):
    print "Message received: "  + message.payload
    msg = message.payload
    name,service_name,service,characteristic,value = msg.split(":")
    parsed_json = {"name": "","service_name": "", "service": "", "characteristic": "", "value":""}
    parsed_json["name"] = name
    parsed_json["service_name"] = service_name
    parsed_json["service"] = service
    parsed_json["characteristic"] = characteristic
    parsed_json["value"] = int(value)
    parsed_json2 = json.dumps(OrderedDict(parsed_json))
    print parsed_json2

    os.system("mosquitto_pub -h 172.20.10.3 -m '" + parsed_json2 + "' -t homebridge/to/set")
    # client.publish("homebridge/to/set", parsed_json2)


Connected = False  # global variable for the state of the connection

broker_address = "172.20.10.3"  # Broker address
port = 1883  # Broker port

client = mqttClient.Client()  # create new instance
client.on_connect = on_connect  # attach function to callback
client.on_message = on_message  # attach function to callback

client.connect(broker_address, port=port)  # connect to broker
client.loop_start()        #start the loop

while Connected != True:    #Wait for connection
    time.sleep(0.1)

client.subscribe("homebridge/to/set/cortina")

try:
    while True:
        time.sleep(1)

except KeyboardInterrupt:
    print "exiting"
    client.disconnect()
    client.loop_stop()