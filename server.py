import asyncio
from flask import Flask, render_template, request
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

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html')


@app.route('/morningLight', methods=['POST'])
def turnOnMLight():
    color = request.form.get('morningColor')
    print("turning on morning light as %s", color)

    color= request.form.get("morningColor", "#FFFFFF")
    color= color.lstrip("#")
    r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    return f"Turing on Morning Light as {color}"

@app.route('/nightLight', methods=['POST'])
def turnOnNLight():
    color = request.form.get('nightColor')
    color= request.form.get("morningColor", "#FFFFFF")
    color= color.lstrip("#")
    r, g, b = (int(color[i:i+2], 16) for i in (0, 2, 4))
    return f"Turing on Night Light as {color}"
    


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