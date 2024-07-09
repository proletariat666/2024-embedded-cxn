from machine import Pin
import time
import network
import umqtt
 
p5=Pin(5,Pin.OUT)
 
w=network.WLAN(network.STA_IF)
w.active(True)
w.connect("zhetu","zhetu123")
 
def mqtt_callback(topic,data):    #回调函数，直接把主题名和内容体打印出来
    print(topic,data)
 
while not w.isconnected():    #确保WiFI连接上了，后续才可以连接MQTT服务器
    pass
    
mqtt=umqtt.MQTTClient("连接服务器用的ID","mqtt服务器IP")
mqtt.set_callback(mqtt_callback)
mqtt.connect()
mqtt.subscribe(b"aaa")
 
while True:
    mqtt.check_msg()            #检测是否接受到订阅信息
    if w.isconnected():
        p5.off()
    else:
        p5.on()
    time.sleep(1)
 