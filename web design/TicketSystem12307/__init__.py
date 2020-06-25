import os
from flask import Flask, url_for, render_template, request, redirect, session, flash, json, jsonify
from TicketSystem12307 import client
from TicketSystem12307.const import * 
import socket

app = Flask(__name__) 
app.config['SECRET_KEY'] = os.getenv('TickeySystem2020', 'SECRET_KEY')

def communicate(command):
    print("try to connect")
    c = client.Client()
    print("send to server >>> " + command)
    msg = c.communicate(command)
    print("recv from server << " + msg)
    return msg

from TicketSystem12307 import views, errors, commands

    