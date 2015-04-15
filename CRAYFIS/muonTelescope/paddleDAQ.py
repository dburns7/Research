import serial
import sys
import time
import datetime

port = sys.argv[1];

try:
  ser = serial.Serial(port, 115200, timeout=1)
except IOError:
  print port, ' CANNOT BE OPENED. TERMINATING SCRIPT';
  sys.exit();

aFile = open('logFile', 'w')

initTime = time.time()

while (1):
    line = ser.readline();
    if len(line) > 0:
        timeNow = time.time();
        timeNowStr = str(timeNow);
        printMe = timeNowStr+":"+line;
        aFile.write(printMe);
        sys.stdout.write(printMe);

ser.close()
