from flask import Flask, render_template, request
import serial 

app = Flask(__name__)
app.config["SECERT_KEY"] = "mycoolawesomekey"
# ser = serial.Serial("Aduino", 9200)

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html')

@app.route('/morningLight', methods=['POST'])
def turnOnMLight():
    color = request.form.get('morningColor')
    print("turning on morning light as %s", color)
    #ser.write(b'H') #I have no clue what this is doing, trying stuff out

@app.route('/nightLight', methods=['POST'])
def turnOnNLight():
    color = request.form.get('nightColor')
    print("turning on night light %s", color)
    #ser.write(b'H') #I have no clue what this is doing, trying stuff out
    ser.write(b'H')
    return '', 204


@app.route('/bluetooth', methods=['POST'])
def getBlutooth():
    data = request.get_json()
    device_name = data.get("deviceName")
    print("We Have Recieved The Device Name: ", device_name)
    # Do this later and make serial have the device name
    return '', 204



if __name__ == '__main__':
    # ssl_context=('cert.pem', 'key.pem')
     app.run(debug=True)