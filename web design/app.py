from flask import Flask, url_for, render_template, request, redirect, session, flash
from client.client import Client
from const import *
from user import * 

app = Flask(__name__) 

@app.route('/')
def index():
    if C_USERNAME in session:
        return render_template('index.html', C_NAME=session[C_NAME])
    else:
        return render_template('index.html')

@app.route('/logout')
def logout():
    if C_USERNAME in session:
        client = Client()
        result = client.communicate("logout -u {}".format(session[C_USERNAME]))
        session.clear()
        if result != "0":
            pass
    return redirect(url_for('index'))
    
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        # print(username, password)

        client = Client()
        try:
            result = client.communicate("login -u {} -p {}".format(username, password))
            print("result = {}".format(type(result)))
            if (result == "0\n"):
                print("result = {}".format(type(result)))
                result = client.communicate("query_profile -c {} -u {}".format(username, username)).split()
                username    = result[0]
                name        = result[1]
                mailAddr    = result[2]
                privilege   = result[3]
                session[C_USERNAME] = username
                session[C_NAME]     = name
                return redirect(url_for('index'))
            else:
                flash("error")
                return render_template('login.html')
        except:
            pass
        
                
        return redirect(url_for('index', _method='GET'))
    else:
        return render_template('login.html')
    
@app.errorhandler(404)
def page_not_found(e):
    return render_template('errors/404.html')
    
@app.route("/terminal")
def terminal():
    pass

app.config.from_object('config')
if __name__ == '__main__':
    app.run(port=5000, debug=True)