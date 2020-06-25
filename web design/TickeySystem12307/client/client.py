import socket

class Client:
    def __init__(self, HOST = "localhost", PORT = 12306, MAXBUFF = 16384):
        self.HOST = HOST 
        self.PORT = PORT 
        self.ADDRESS = (self.HOST, self.PORT)
        self.MAXBUFF = MAXBUFF

    def communicate(self, data):
        total_data = []
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect(self.ADDRESS)
            s.sendall(data.encode())
            while True:
                data = s.recv(self.MAXBUFF).decode()
                if not data:
                    break
                total_data.append(data)
        return ''.join(total_data)

