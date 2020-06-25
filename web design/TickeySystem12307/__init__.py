import os
from flask import Flask, url_for, render_template, request, redirect, session, flash, json, jsonify
from client.client import Client
from const import * 
import socket

app = Flask(__name__) 
app.config['SECRET_KEY'] = os.getenv('TickeySystem2020', 'SECRET_KEY')

from TickeySystem12307 import views, errors, commands

app.config.from_object('config')
if __name__ == '__main__':
    app.run(port=5000, debug=True)
    