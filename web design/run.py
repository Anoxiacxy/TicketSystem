from gevent.pywsgi import WSGIServer
from TicketSystem12307 import app

http_server = WSGIServer(('', 5000), app)
http_server.serve_forever()