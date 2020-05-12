#!/usr/bin/python3
from flask import Flask, render_template, request, redirect
from werkzeug.utils import secure_filename
from json import load

# https://plotly.com/javascript/reference/

#
# CONSTANT
#
HOST = "127.0.0.1"

#
# GLOBAL
#

app = Flask(__name__)

#
# ROUTING
#
@app.route('/')
def mainpage():
    """Render main page"""
    return render_template('index.html')

@app.route('/stats')
def teststats():
    try:
        with open("/home/nvpb6652/Documents/code/dev/LatSeq/journeys.23042020.json") as f:
            jj = load(f)
    except Exception as e:
        print(e)
        return redirect('/')
    else:
        tmp_jd = jj['D']
        tmp_ju = jj['U']
        size_jd = tmp_jd['size']
        size_ju = tmp_ju['size']
        return render_template(
            'stats.html'
        )

#
# MAIN
#
if __name__ == "__main__":
    """Run web app"""
    app.run(host=HOST, debug=True)
