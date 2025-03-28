from flask import Flask, render_template, request, redirect, url_for, jsonify
from flask_sqlalchemy import *
from flask_login import *
"""
#https://stackoverflow.com/questions/24214643/python-to-automatically-select-serial-ports-for-arduino
"""
app = Flask(__name__)
app.config["SECRET_KEY"] = "mycoolawesomekey"

app = Flask(__name__)
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
        return render_template("after.html")
    else:
        return render_template('index.html')

@app.route('/morningLight', methods=['POST'])
def turnOnMLight():
    # color= request.form.get("morningColor", "#FFFFFF")
    # color= color.lstrip("#")
    # r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    # ser.write(f"{r},{g},{b}\n".encode())
    return " ", 200

@app.route('/nightLight', methods=['POST'])
def turnOnNLight():
    color= request.form.get("morningColor", "#FFFFFF")
    color= color.lstrip("#")
    r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    return f"Turing on Night Light as {color}"

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


if __name__ == '__main__':
    # ssl_context=('cert.pem', 'key.pem')
     app.run(debug=True)