import socket
import threading


HEADER = 64
PORT = 5050
#SERVER = "192.168.43.1"
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER,PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"


Server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
Server.bind(ADDR)


def handle_client(conn,addr):
    print(f"New Connection {addr} connected ")

    connected = True
    while connected:
        msg__ = conn.recv(2048).decode(FORMAT)
        print(msg__)
        conn.send("Meg Received".encode(FORMAT))
        # msg_length = conn.recv(HEADER).decode(FORMAT)
        # if msg_length:
        #     msg_length = int(msg_length)
        #     msg = conn.recv(msg_length).decode(FORMAT)
        #     if msg == DISCONNECT_MESSAGE:
        #         connected = False
        #     print(f"[{addr}] {msg}")
        #     conn.send("Meg Received".encode(FORMAT))

connection = 0

def SEND():
    while True:
        data = input('Enter data')
        connection.send(data.encode(FORMAT))

def start():
    Server.listen()
    print(f"[Listening] server is listening on {SERVER}")
    while True:
        conn , addr =  Server.accept()
        global connection
        connection = conn
        thread = threading.Thread(target =handle_client, args=(conn, addr))
        thread.start()
        DATA = threading.Thread(target=SEND)
        DATA.start()
        print(f"[Active Connections] {threading.activeCount() - 1}")

print("[Starting] the server is starting ...")
start()


#print(SERVER)


