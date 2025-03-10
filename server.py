import asyncio
from flask import Flask, render_template, request, redirect, url_for, jsonify
from flask_sqlalchemy import *
from flask_login import *
from bleak import BleakClient
from bleak import discover
import serial.tools.list_ports
"""
#https://stackoverflow.com/questions/24214643/python-to-automatically-select-serial-ports-for-arduino
"""
app = Flask(__name__)
app.config["SECRET_KEY"] = "mycoolawesomekey"

characteristic ='c4d6aa1d-e90b-42fa-977e-039e7b401994'
'''
************************************
This is Stuff for arduino
'''
# https://panamahitek.com/en/wireless-communication-between-arduino-and-python-using-ble/
# Asynchronous function to scan for BLE devices
# async def scan():
#     # Use the discover function from the bleak library to scan for devices
#     devices = await discover()

#     # Loop through the list of discovered devices
#     for d in devices:
#         if(d.name and "Arduino" in d.name):
#             print(d)
# loop = asyncio.get_event_loop()
# loop.run_until_complete(scan())

'''
************************************
'''
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
    if request.method == "POST":
        print("we got here!!!")
        jsonData = request.get_json()
        print(jsonData)
    # color= request.form.get("morningColor", "#FFFFFF")
    # color= color.lstrip("#")
    # r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    # ser.write(f"{r},{g},{b}\n".encode())
    return render_template('index.html')

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

@app.route('/bluetooth', methods=['POST'])
def getBlutooth():
    address = "9F83D883-81AA-5450-5C2A-5F7A1C9B5E4A"
    data = 'B'
    ''' https://stackoverflow.com/questions/61543406/asyncio-run-runtimeerror-event-loop-is-closed '''
    ## ALWAYS COME BACK TO THIS - if bluetooth is not working for some reason this is valid code now ##
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(main(address=address, data=data))
    loop.close()
    return " ", 200

async def main(address, data):
    client = BleakClient(address)
    try:
        await client.connect()
        await client.write_gatt_char(characteristic, data.encode('utf-8'))
    except Exception as e:
        print(e)

if __name__ == '__main__':
    # ssl_context=('cert.pem', 'key.pem')
     app.run(debug=True)