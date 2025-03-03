from flask import Flask, render_template, request
#import serial 

app = Flask(__name__)
app.config["SECERT_KEY"] = "mycoolawesomekey"
#ser = serial.Serial("COM6", 9600)

@app.route('/', methods=['GET', 'POST'])
def index():
    #return 'hello world'
    return render_template('index.html')


@app.route('/morningLight', methods=['POST'])
def turnOnMLight():
    color = request.form.get('morningColor')
    return ("turning on morning light as %s", color)
    #ser.write(b'H') #I have no clue what this is doing, trying stuff out

@app.route('/nightLight', methods=['POST'])
def turnOnNLight():
    color = request.form.get('nightColor')
    print("turning on night light %s", color)
    #ser.write(b'H') #I have no clue what this is doing, trying stuff out

if __name__ == '__main__':
     app.run(debug=True)