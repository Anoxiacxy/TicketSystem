@app.route("/ajax_terminal", methods=["POST"])
def ajax_terminal():
    data = json.loads(request.form.get('data'))
    try:
        command = data['command']
        result = communicate(command)
        
        return jsonify({
            "result" : "success",
            "response" : result
        })
    except:
        return jsonify({"result" : "error"})

@app.route("/ajax_add_user", methods=["POST"])
def ajax_add_user():
    data = json.loads(request.form.get('data'))
    try:
        command = "add_user"
        for opt in ["-c", "-u", "-p", "-n", "-m", "-g"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })    
        
        result = communicate(command).split()
        if len(result) == 1 and result[0] == "0":
            return jsonify({
                "result" : "success"
            })
        else:
            return jsonify({ "result" : "error" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_login", methods=["POST"])
def ajax_login():
    data = json.loads(request.form.get('data'))
    try:
        command = "login"
        for opt in ["-u", "-p"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        
        result = communicate(command).split()
        if len(result) == 1 and result[0] == "0":
            return jsonify({
                "result" : "success"
            })
        else:
            return jsonify({ "result" : "error" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_logout", methods=["POST"])
def ajax_logout():
    data = json.loads(request.form.get('data'))
    try:
        command = "logout"
        for opt in ["-u"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        
        result = communicate(command).split()
        if len(result) == 1 and result[0] == "0":
            return jsonify({
                "result" : "success"
            })
        else:
            return jsonify({ "result" : "error" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_query_profile", methods=["POST"])
def ajax_query_profile():
    data = json.loads(request.form.get('data'))
    try:
        command = "query_profile"
        for opt in ["-c", "-u"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        
        result = communicate(command).split()
        if len(result) == 4:
            return jsonify({
                "result" : "success",
                C_USERNAME : result[0],
                C_NAME : result[1],
                C_MAILADDR : result[2],
                C_PRIVILEGE : result[3]
            })
        else:
            return jsonify({ "result" : "error" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_modify_profile", methods=['POST'])
def ajax_modify_profile():
    data = json.loads(request.form.get('data'))
    try:
        command = "modify_profile" + " -c " + data['-c'] + " -u " + data['-u']

        for opt in ["-c", "-u"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })

        for opt in ["-p", "-n", "-m", "-g"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
                
        result = communicate(command).split()
        if len(result) == 4:
            username    = result[0]
            name        = result[1]
            mailAddr    = result[2]
            privilege   = result[3]
            if (data["-c"] == data["-u"]):
                session[C_USERNAME] = username
                session[C_NAME]     = name
                session[C_MAILADDR] = mailAddr
                session[C_PRIVILEGE] = privilege

            return jsonify({
                "result"   :   "success",
                C_USERNAME  :   username, 
                C_NAME      :   name, 
                C_MAILADDR  :   mailAddr,
                C_PRIVILEGE :   privilege
            })
        else:
            return jsonify({
                "result"     :   "error"
            })
    except:
        return jsonify({"result" : "error"})
        
@app.route("/ajax_add_train", methods=["POST"])
def ajax_add_train():
    data = json.loads(request.form.get('data'))
    try:
        command = "add_train"
        for opt in ["-i", "-n", "-m", "-s", "-p", "-x", "-t", "-o", "-d", "-y"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        
        result = communicate(command).split()
        if len(result) == 1 and result[0] == "0":
            return jsonify({
                "result" : "success",
            })
        else:
            return jsonify({ "result" : "error" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_release_train", methods=["POST"])
def ajax_release_train():
    data = json.loads(request.form.get('data'))
    try:
        command = "release_train"
        for opt in ["-i"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        
        result = communicate(command).split()
        if len(result) == 1 and result[0] == "0":
            return jsonify({
                "result" : "success",
            })
        else:
            return jsonify({ "result" : "error" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_query_train", methods=["POST"])
def ajax_query_train():
    data = json.loads(request.form.get('data'))
    try:
        command = "query_train"
        for opt in ["-i", "-d"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        
        result = communicate(command).split()
        if len(result) != 1 or result[0] != "-1":
            station = []
            for i in range(2, len(result), 8):
                content = {
                    "station" : result[i],
                    "ARRIVING_TIME" : result[i + 1] + " " + result[i + 2],
                    "LEAVING_TIME" : result[i + 4] + " " + result[i + 5],
                    "PRICE" : result[i + 6],
                    "SEAT" : result[i + 7]
                }
                station.append(content)

            return jsonify({
                "result" : "success",
                "trainID" : result[0],
                "type" : result[1],
                "stations" : station
            })
        else:
            return jsonify({ "result" : "error" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_delete_train", methods=["POST"])
def ajax_delete_train():
    data = json.loads(request.form.get('data'))
    try:
        command = "delete_train"
        for opt in ["-i"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        
        result = communicate(command).split()
        if len(result) == 1 and result[0] == "0":
            return jsonify({
                "result" : "success",
            })
        else:
            return jsonify({ "result" : "error" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_query_ticket", methods=["POST"])
def ajax_query_ticket():
    data = json.loads(request.form.get('data'))
    try:
        command = "query_ticket"
        for opt in ["-s", "-t", "-d"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        for opt in ["-p"]:
            if opt in data:
                command += " " + opt + " " + data[opt]

        result = communicate(command).split()
        if len(result) == 1 and result[0] == "-1":
            return jsonify({ "result" : "error" })
        else:
            trains = []
            for i in range(1, len(result), 10):
                content = {
                    "trainID" : result[i],
                    "FROM" : result[i + 1],
                    "LEAVING_TIME" : result[i + 2] + " " + result[i + 3],
                    "TO" : result[i + 5],
                    "ARRIVING_TIME" : result[i + 6] + " " + result[i + 7],
                    "PRICE" : result[i + 8],
                    "SEAT" : result[i + 9]
                }
                trains.append(content)
            return jsonify({
                "result" : "success",
                "number" : result[0],
                "trains" : trains
            })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_query_transfer", methods=["POST"])
def ajax_query_transfer():
    data = json.loads(request.form.get('data'))
    try:
        command = "query_transfer"
        for opt in ["-s", "-t", "-d"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })
        for opt in ["-p"]:
            if opt in data:
                command += " " + opt + " " + data[opt]

        result = communicate(command).split()
        if len(result) == 1 and result[0] == "0":
            return jsonify({ "result" : "error" })
        else:
            trains = []
            for i in range(0, len(result), 10):
                content = {
                    "trainID" : result[i],
                    "FROM" : result[i + 1],
                    "LEAVING_TIME" : result[i + 2] + " " + result[i + 3],
                    "TO" : result[i + 5],
                    "ARRIVING_TIME" : result[i + 6] + " " + result[i + 7],
                    "PRICE" : result[i + 8],
                    "SEAT" : result[i + 9]
                }
                trains.append(content)
            return jsonify({
                "result" : "success",
                "number" : "2",
                "trains" : trains
            })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_buy_ticket", methods=["POST"])
def ajax_buy_ticket():
    print("1")
    data = json.loads(request.form.get('data'))
    print("2")
    try:
        print("3")
        command = "buy_ticket"
        print("4")
        for opt in ["-u", "-i", "-d", "-n", "-f", "-t"]:
            print("5" + opt)
            if opt in data:
                print("50" + opt)
                print(data[opt])
                command += " " + opt + " " + data[opt]
                print("5000" + opt)
            else:
                print("51" + opt)
                return jsonify({ "result" : "error" })
        print("5")
        for opt in ["-q"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
        print("6")
        result = communicate(command).split()
        if result[0] == "-1":
            return jsonify({ "result" : "error" })
        elif result[0] == "queue":
            return jsonify({ "result" : "queue" })
        else:
            return jsonify({
                "result" : "success",
                "PRICE" : result[0]
            })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_query_order", methods=["POST"])
def ajax_query_order():
    data = json.loads(request.form.get('data'))
    try:
        command = "query_order"
        for opt in ["-u"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })

        result = communicate(command).split()
        if len(result) == 1 and result[0] == "-1":
            return jsonify({ "result" : "error" })
        else:
            orders = []
            for i in range(1, len(result), 11):
                content = {
                    "STATUS" : result[i][1:-1],
                    "trainID" : result[i + 1],
                    "FROM" : result[i + 2],
                    "LEAVING_TIME" : result[i + 3] + " " + result[i + 4],
                    "TO" : result[i + 6],
                    "ARRIVING_TIME" : result[i + 7] + " " + result[i + 8],
                    "PRICE" : result[i + 8],
                    "NUM" : result[i + 10]
                }
                orders.append(content)
            return jsonify({
                "result" : "success",
                "number" : result[0],
                "orders" : orders
            })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_refund_ticket", methods=["POST"])
def ajax_refund_ticket():
    data = json.loads(request.form.get('data'))
    try:
        command = "refund_ticket"
        for opt in ["-u"]:
            if opt in data:
                command += " " + opt + " " + data[opt]
            else:
                return jsonify({ "result" : "error" })

        for opt in ["-n"]:
            if opt in data:
                command += " " + opt + " " + data[opt]

        result = communicate(command).split()
        if len(result) == 1 and result[0] == "-1":
            return jsonify({ "result" : "error" })
        else:
            return jsonify({
                "result" : "success",
            })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_clean", methods=["POST"])
def ajax_clean():
    data = json.loads(request.form.get('data'))
    try:
        command = "clean"
        result = communicate(command).split()
        return jsonify({ "result" : "success" })
    except:
        return jsonify({ "result" : "error" })

@app.route("/ajax_exit", methods=["POST"])
def ajax_exit():
    data = json.loads(request.form.get('data'))
    try:
        command = "exit"

        result = communicate(command).split()
        return jsonify({ "result" : "success" })
    except:
        return jsonify({ "result" : "error" })