import serial
import time
 
s = serial.Serial('/dev/ttyACM0', 9600) # change name, if needed
# s.open()
time.sleep(5) # the Arduino is reset after enabling the serial connectio, therefore we have to wait some seconds
 
s.write("test".encode('utf-8', errors='ignore'))
try:
    while True:
        response = s.readline()
        print(type(response))
        result = response.decode('utf-8', errors='ignore')
        print(result, type(result))
except KeyboardInterrupt:
    s.close()
