from flask import Flask, url_for, render_template, request, redirect, session, flash, json, jsonify
from client.client import Client
from const import *
from user import * 
import socket

app = Flask(__name__) 

# https://secure.gravatar.com/avatar/md5

@app.route('/')
def index():
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('index.html', argv=argv)

def communicate(command):
    client = Client()
    return client.communicate(command)

@app.route('/logout')
def logout():
    if C_USERNAME in session:
        result = communicate("logout -u {}".format(session[C_USERNAME]))
        session.clear()
        if result != "0":
            pass
    argv={}
    return redirect(url_for('index'))

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        
        argv={}
        argv[C_USERNAME]=username
        argv[C_PASSWORD]=password
        message = ""
        try:
            result = communicate("login -u {} -p {}".format(username, password)).split()
            print("result = {}".format(result))
            
            if len(result) == 1 and result[0] == "0":
                print("result = {}".format(type(result)))
                result = communicate("query_profile -c {} -u {}".format(username, username)).split()
                username    = result[0]
                name        = result[1]
                mailAddr    = result[2]
                privilege   = result[3]
                session[C_USERNAME] = username
                session[C_NAME]     = name
                session[C_MAILADDR] = mailAddr
                session[C_PRIVILEGE] = privilege
                return redirect(url_for('index'))
            else:
                message = "密码错误"
        except ConnectionRefusedError:
            message = "拒绝访问"   
        except socket.timeout:
            message = "连接超时"
        except SyntaxError:
            message = "未知错误"
        
        for key in session:
            argv[key] = session[key]
        argv['modal_header']='错误信息'
        argv['modal_content']=message
        argv['show_modal']=True
        return render_template('login.html', argv=argv)
    else:
        argv={}
        for key in session:
            argv[key] = session[key]
        return render_template('login.html', argv=argv)
    
@app.route('/profile')
def profile():
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('profile.html', argv=argv)

@app.route('/about')
def about():
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('about.html', argv=argv)

@app.route('/buy_ticket')
def buy_ticket():
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('buy_ticket.html', argv=argv)

@app.route('/train_manage')
def train_manage():
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('about.html', argv=argv)



@app.errorhandler(404)
def page_not_found(e):
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('errors/404.html', argv=argv)
    
@app.route("/terminal")
def terminal():
    pass

@app.route("/ajax_query_profile", methods=["POST"])
def ajax_query_profile():
    pass

@app.route("ajax_add_user", methods=['POST'])
def ajax_add_user():
    data = json.loads(request.form.get('data'))
    command = 

@app.route("/ajax_modify_profile", methods=['POST'])
def ajax_modify_profile():
    data = json.loads(request.form.get('data'))
    try:
        command = "modify_profile" + " -c " + data['-c'] + " -u " + data['-u']

        for opt in ["-p", "-n", "-m", "-g"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
                
        result = communicate(command).split()
        print(result)
        if len(result) == 4:
            username    = result[0]
            name        = result[1]
            mailAddr    = result[2]
            privilege   = result[3]
            session[C_USERNAME] = username
            session[C_NAME]     = name
            session[C_MAILADDR] = mailAddr
            session[C_PRIVILEGE] = privilege
            #print("a")
            return jsonify({
                "type"   :   "success",
                C_USERNAME  :   username, 
                C_NAME      :   name, 
                C_MAILADDR  :   mailAddr,
                C_PRIVILEGE :   privilege
            })
        else:
            #print("b")
            return jsonify({
                "type"     :   "error"
            })
    except:
        return jsonify({"type" : "error"})
        
 


app.config.from_object('config')
if __name__ == '__main__':
    app.run(port=5000, debug=True)