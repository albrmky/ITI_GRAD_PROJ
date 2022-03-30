import serial
import serial.tools.list_ports




com_port = "COM7" #leave is empty to detect automatically for prolific
com_port_baudrate = 9600

record = ":10000000005000200519000855190008551900086E\n"


# print the used port
print('prolific USB-toSerial is detected on ',com_port)

# open the port
ser = serial.Serial(com_port,com_port_baudrate)



ser.write(record.encode())
