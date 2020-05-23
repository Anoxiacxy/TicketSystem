from flask import Flask
from flask import url_for
from flask import render_template
from flask import request
from flask import redirect

app = Flask(__name__)
    
name	=	'Grey	Li' 
movies	=	[				
    {'title':	'My	Neighbor	Totoro',	'year':	'1988'},				
    {'title':	'Dead	Poets	Society',	'year':	'1989'},				
    {'title':	'A	Perfect	World',	'year':	'1993'},				
    {'title':	'Leon',	'year':	'1994'},				
    {'title':	'Mahjong',	'year':	'1996'},				
    {'title':	'Swallowtail	Butterfly',	'year':	'1996'},				
    {'title':	'King	of	Comedy',	'year':	'1999'},				
    {'title':	'Devils	on	the	Doorstep',	'year':	'1999'},				
    {'title':	'WALL-E',	'year':	'2008'},				
    {'title':	'The	Pork	of	Music',	'year':	'2012'}, 
]



@app.route('/')
def index():
    return render_template('index.html', name = name, movies = movies)

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        return redirect(url_for('index', _method='GET'))
    else:
        return render_template('login.html')

@app.route('/bootstrap')
def bootstrap():
    return render_template('bootstrap.html')
    
@app.route('/responsive')
def responsive():
    return render_template('responsive.html')
    
@app.route('/sticky')
def sticky():
    return render_template('sticky.html')

    
#@app.errorhandler(404)
#def page_not_found(e):
    
