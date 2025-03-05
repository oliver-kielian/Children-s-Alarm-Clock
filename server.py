from flask import Flask, render_template, request
import serial 
import serial.tools.list_ports

#https://stackoverflow.com/questions/24214643/python-to-automatically-select-serial-ports-for-arduino

app = Flask(__name__)
app.config["SECERT_KEY"] = "mycoolawesomekey"

ser = serial.Serial()

@app.route('/', methods=['GET', 'POST'])
def index():
    #return 'hello world'
    return render_template('index.html')


@app.route('/morningLight', methods=['POST'])
def turnOnMLight():
    color= request.form.get("morningColor", "#FFFFFF")
    color= color.lstrip("#")
    r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    ser.write(f"{r},{g},{b}\n".encode())
    return f"Turing on Morning Light as {color}"

@app.route('/nightLight', methods=['POST'])
def turnOnNLight():
    color = request.form.get('nightColor')
    color= request.form.get("morningColor", "#FFFFFF")
    color= color.lstrip("#")
    r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    ser.write(f"{r},{g},{b}\n".encode())
    return f"Turing on Night Light as {color}"
    


@app.route('/bluetooth', methods=['POST'])
def getBlutooth():
    data = request.get_json()
    device_name = data.get("deviceName")
    print("We Have Recieved The Device Name: ", device_name)
    arduino_ports = [
    p.device
    for p in serial.tools.list_ports.comports()
    if 'Arduino' in p.description ]
    ser = serial.Serial(arduino_ports[0], 9600)
    # Do this later and make serial have the device name
    return '', 204

if __name__ == '__main__':
    # ssl_context=('cert.pem', 'key.pem')
     app.run(debug=True)