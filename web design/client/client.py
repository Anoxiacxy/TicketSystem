import socket

class Client:
    def __init__(self, HOST = "localhost", PORT = 12306, MAXBUFF = 4096):
        self.HOST = HOST 
        self.PORT = PORT 
        self.ADDRESS = (self.HOST, self.PORT)
        self.MAXBUFF = MAXBUFF

    def communicate(self, data):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect(self.ADDRESS)
            s.sendall(data.encode())
            data = s.recv(self.MAXBUFF).decode()
        return data

