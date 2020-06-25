from TicketSystem12307 import *

@app.route('/')
def index():
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('index.html', argv=argv)

@app.route('/logout')
def logout():
    if IS_LOGED_IN in session:
        result = communicate("logout -u {}".format(session[C_USERNAME]))
        session.clear()
        if result != "0":
            pass
    argv={}
    return redirect(url_for('index'))

@app.route('/register')
def register():
    if IS_LOGED_IN in session:
        return redirect(url_for('index'))
    else:
        argv={}
        for key in session:
            argv[key] = session[key]
        return render_template('register.html', argv=argv)

@app.route('/login', methods=['GET', 'POST'])
def login():
    if IS_LOGED_IN in session:
        return redirect(url_for('index'))

    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        
        argv={}
        argv[C_USERNAME]=username
        argv[C_PASSWORD]=password
        message = ""
        try:
            result = communicate("login -u {} -p {}".format(username, password)).split()
            
            if len(result) == 1 and result[0] == "0":
                result = communicate("query_profile -c {} -u {}".format(username, username)).split()
                username    = result[0]
                name        = result[1]
                mailAddr    = result[2]
                privilege   = result[3]
                session[C_USERNAME] = username
                session[C_NAME]     = name
                session[C_MAILADDR] = mailAddr
                session[C_PRIVILEGE] = privilege
                session[IS_LOGED_IN] = True
                if int(privilege) > 5:
                    session[IS_MANAGER] = True
                return redirect(url_for('index'))
            else:
                message = "密码错误或您已在别处登陆"
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
    if IS_LOGED_IN not in session:
        return redirect(url_for('login'))   
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('profile.html', argv=argv)

@app.route('/orders')
def orders():
    if IS_LOGED_IN not in session:
        return redirect(url_for('login'))   
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('orders.html', argv=argv)

@app.route('/about')
def about():
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('about.html', argv=argv)

@app.route('/buy_ticket')
def buy_ticket():
    if IS_LOGED_IN not in session:
        return redirect(url_for('login'))   
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('buy_ticket.html', argv=argv)

@app.route('/train_manage')
def train_manage():
    if IS_LOGED_IN not in session or IS_MANAGER not in session:
        return redirect(url_for('login'))   
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('train_manage.html', argv=argv)

@app.route('/user_manage')
def user_manage():
    if IS_LOGED_IN not in session or IS_MANAGER not in session:
        return redirect(url_for('login'))   
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('user_manage.html', argv=argv)
