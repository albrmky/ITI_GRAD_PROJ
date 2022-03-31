import socket
import threading
import read_hex


HEADER = 64
PORT = 5050
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER,PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

file = open("D:/_NOOUR_/ITI_ES/ARM_workspace/Simple_blinking/Debug/Simple_blinking.hex","r")
file2 = open("D:/_NOOUR_/ITI_ES/ARM_workspace/Simple_blinking_2/Debug/Simple_blinking_2.hex","r")

records = list()
Server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
Server.bind(ADDR)


def handle_client(conn,addr):
    global records
    print(f"New Connection {addr} connected ")

    connected = True
    while connected:
        var = conn.recv(2048).decode(FORMAT)
        print(var)
        if var == DISCONNECT_MESSAGE:
            connected = False
            break

        print(var)

        if var == "0x11\n":
            records = [line[1:] for line in file2]

        elif var == "0x22\n":
            records = [line[1:] for line in file]

        print("hex file consist of: ", len(records), "record")

        succ_lines = 0
        current_line = 1 # notepad start from 0 but in fact that is element 0 in the list

        if var == "0x11\n":
            conn.send("Version = 4.2\n".encode())
        elif var == "0x22\n":
            conn.send("Version = 4.1\n".encode())

        if read_hex.check_hex_file(records) == 0:
            for line in records:
                s = ":" + line
                conn.send(s.encode())
                var = conn.recv(20).decode(FORMAT).lower()[0:-1]  # to receive ok
                print(var)
                if var == DISCONNECT_MESSAGE:
                    connected = False

                print(var, "for line :", current_line)
                current_line += 1
                if var.lower() == "ok":
                    # print(line)
                    succ_lines += 1

        print("succ lines", succ_lines)


connection = 0


def start():
    Server.listen()
    print(f"[Listening] server is listening on {SERVER}")
    while True:
        conn , addr =  Server.accept()
        global connection
        connection = conn
        thread = threading.Thread(target =handle_client, args=(conn, addr))
        thread.start()
        print(f"[Active Connections] {threading.activeCount() - 1}")


print("[Starting] the server is starting ...")
start()



