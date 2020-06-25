@app.errorhandler(404)
def page_not_found(e):
    argv={}
    for key in session:
        argv[key] = session[key]
    return render_template('errors/404.html', argv=argv)