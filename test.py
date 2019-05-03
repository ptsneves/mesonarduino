import pexpect
import os
import sys
import time

power_cmd = "socat -t0  STDIO,raw,echo=0,escape=0x03,nonblock=1,b57600,end-close file:/tmp/myserial,b57600,cs8,parenb=0,cstopb=0,clocal=0,raw,echo=0"
serialconn = pexpect.spawnu(power_cmd, timeout=2, env=os.environ,
    logfile=sys.stdout)
serialconn.send('\r')
serialconn.expect('Waiting for input\r\n')
serialconn.send('BOARD:ON\r')
serialconn.expect('Success: BOARD:ON')
time.sleep(2)
serialconn.send('BOARD:OFF\r')
serialconn.expect('Success: BOARD:OFF')
