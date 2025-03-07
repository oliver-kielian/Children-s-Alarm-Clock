import asyncio
from flask import Flask, render_template, request
<<<<<<< Updated upstream
import serial 

app = Flask(__name__)
app.config["SECERT_KEY"] = "mycoolawesomekey"
# ser = serial.Serial("Aduino", 9200)
=======
from bleak import BleakClient
from bleak import discover

characteristic ='c4d6aa1d-e90b-42fa-977e-039e7b401994'
'''
************************************
This is Stuff for arduino
'''
# https://panamahitek.com/en/wireless-communication-between-arduino-and-python-using-ble/
# Asynchronous function to scan for BLE devices
async def scan():
    # Use the discover function from the bleak library to scan for devices
    devices = await discover()

    # Loop through the list of discovered devices
    for d in devices:
        if(d.name and "Arduino" in d.name):
            print(d)
loop = asyncio.get_event_loop()
loop.run_until_complete(scan())

'''
************************************
'''


app = Flask(__name__)
app.config["SECERT_KEY"] = "mycoolawesomekey"
DEVICE_NAME = 'Arduino'
>>>>>>> Stashed changes

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html')

@app.route('/morningLight', methods=['POST'])
def turnOnMLight():
<<<<<<< Updated upstream
    color = request.form.get('morningColor')
    print("turning on morning light as %s", color)
    #ser.write(b'H') #I have no clue what this is doing, trying stuff out
=======
    color= request.form.get("morningColor", "#FFFFFF")
    color= color.lstrip("#")
    r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    # ser.write(f"{r},{g},{b}\n".encode())
    return f"Turing on Morning Light as {color}"
>>>>>>> Stashed changes

@app.route('/nightLight', methods=['POST'])
def turnOnNLight():
    color = request.form.get('nightColor')
<<<<<<< Updated upstream
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
=======
    color= request.form.get("morningColor", "#FFFFFF")
    color= color.lstrip("#")
    r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    # ser.write(f"{r},{g},{b}\n".encode())
    return f"Turing on Night Light as {color}"
    
@app.route('/bluetooth', methods=['POST'])
def getBlutooth():
    address = "9F83D883-81AA-5450-5C2A-5F7A1C9B5E4A"
    data = "B"
    ''' https://stackoverflow.com/questions/61543406/asyncio-run-runtimeerror-event-loop-is-closed '''
    # Come back to this as this session seems to not be closing and needs work on!!!
    asyncio.set_event_loop(loop)
    return " ", 200

async def main(address, data):
    client = BleakClient(address)
    try:
        await client.connect()
        await client.write_gatt_char(characteristic, data.encode('utf-8'))
    except Exception as e:
        print(e)
>>>>>>> Stashed changes

if __name__ == '__main__':
    # ssl_context=('cert.pem', 'key.pem')
     app.run(debug=True)