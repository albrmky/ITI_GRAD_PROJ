import serial
import read_hex
import serial.tools.list_ports

# open the hex file
file = open("D:/_NOOUR_/ITI_ES/ARM_workspace/Simple_blinking/Debug/Simple_blinking.hex","r")
# get the record and remove the preceding colon
records = [line[1:] for line in file]

com_port = str() #leave is empty to detect automatically for prolific
com_port_baudrate = 115200


my_ports = [str(p) for p in list(serial.tools.list_ports.comports())] # list current ports

# if Com not specified manually search on prolific
if com_port.find('COM') < 0:
    for port in my_ports:
        if port.find('Prolific USB-to-Serial') >= 0:
            com_port = port[0:port.find('-')-1] # extract the com number from port info
            break

# print the used port
print('prolific USB-toSerial is detected on ',com_port)
print("hex file consist of: ",len(records),"record")

# open the port
ser = serial.Serial(com_port,com_port_baudrate)

succ_lines=0
current_line =1
if read_hex.check_hex_file(records) == 0:
    for line in records:
        ser.write(b':')
        ser.write(line.encode().lower())
        var = ser.readline().decode()[0:-1] # to receive ok
        print(var,"for line :",current_line)
        current_line+=1
        if var =="ok":
            #print(line)
            succ_lines+=1


print("succ lines",succ_lines)