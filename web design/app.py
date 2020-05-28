from flask import Flask, url_for, render_template, request, redirect
from client import Client

app = Flask(__name__) 

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        return redirect(url_for('index', _method='GET'))
    else:
        return render_template('login.html')
    
@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html')
    
