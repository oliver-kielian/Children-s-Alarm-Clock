from flask import Flask, render_template, request, redirect, url_for, jsonify
from flask_sqlalchemy import *
from flask_login import *

app = Flask(__name__, static_folder='static')
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///database.db'
app.config['SECRET_KEY'] = 'mycoolawesomekey'
db = SQLAlchemy(app)

class User(UserMixin, db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(40), unique=True, nullable=False)
    username = db.Column(db.String(40), nullable=False)
    password = db.Column(db.String(40), nullable=False)


login_manager = LoginManager(app)
login_manager.init_app(app)
app.app_context().push()
db.create_all()

@login_manager.user_loader
def load_user(uid):
    user = User.query.filter_by(id=uid).first()
    return user

@app.route('/create', methods=['GET', 'POST'])
def create():
    if request.method == "POST":
        name = request.form["name"]
        username = request.form["username"]
        password = request.form["password"]
        user = User(name=name, username=username, password=password)
        if User.query.filter_by(username=username).first():
            error = " "
            return render_template("create.html", error=error)
        else:
            db.session.add(user)
            db.session.commit()
            login_user(user)
        return redirect(url_for('index'))
    else:
        return render_template('create.html')

@app.route('/login', methods=["GET", "POST"])
def login():
    if request.method == "GET":
        return render_template('login.html')
    elif request.method == "POST":
        username = request.form['username']
        password = request.form['password']
        user = User.query.filter_by(username=username).first()
        if user != None and password == password:
            login_user(user)
            return redirect(url_for('index'))
        else:
            msg = " "
            return render_template('login.html', msg=msg)

@app.route('/')
def index():
    if current_user.is_authenticated:
        return render_template("home.html")
    else:
        return render_template('index.html')

@app.route('/morningLight', methods=['POST'])
def turnOnMLight():
    return " ", 200

@app.route('/nightLight', methods=['POST'])
def turnOnNLight():
    return " ", 200

@app.route('/nightAlarm', methods=['POST']) 
def setNightAlarm():
    data = request.get_json()
    time = data.get("time")
    return jsonify({"message": f"Alarm set to {time}"})

@app.route('/morningAlarm', methods=['POST']) 
def setMorningAlarm():
    data = request.get_json()
    time = data.get("time")
    return jsonify({"message": f"Alarm set to {time}"})

@app.route('/home', methods=['GET'])
def home():
    return render_template('home.html')


@app.route('/lights', methods=['GET'])
def lights():
    return render_template('lights.html')

@app.route('/colors', methods=['GET'])
def colors():
    return render_template('colors.html')

@app.route('/account', methods=['GET'])
def account():
    return render_template('account.html')

@app.route('/logout', methods=['POST'])
def logout():
    logout_user()
    return redirect(url_for('index'))


if __name__ == '__main__':
    # ssl_context=('cert.pem', 'key.pem')
     app.run(debug=True)