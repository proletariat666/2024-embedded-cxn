from machine import Pin
import time
import network
 
p5=Pin(5,Pin.OUT)
 
w=network.WLAN(network.STA_IF)
w.active(True)
w.connect("zhetu","zhetu123")
 
while True:
    if w.isconnected():
        p5.off()
    else:
        p5.on()
    time.sleep(1)