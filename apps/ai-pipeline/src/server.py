from flask import Flask, request, send_file

app = Flask(__name__)

# @app.route('/', method=['GET','POST'])


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug = True, port=5100)