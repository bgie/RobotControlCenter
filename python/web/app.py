import eventlet
import subprocess
import urllib.parse
import sys
import signal
import time
import os
from flask import Flask, render_template, redirect, url_for, request, session, escape, Markup
from flask_socketio import SocketIO
from pathlib import Path

eventlet.monkey_patch()
app = Flask(__name__)
app.secret_key = "FXwvassq5txgHzoFkcYV"
socketio = SocketIO(app)
threads = {}
scripts_path = str(Path.home() / Path('robotcontrolcenter'))
try:
    os.makedirs(scripts_path, exist_ok=True)
    os.symlink(os.path.abspath('../robots'), scripts_path + '/robots', target_is_directory=True)
except FileExistsError:
    pass


# Redirect to the login page if not authenticated
def require_authentication(fn):
    def decorator(**kwargs):
        if 'username' not in session:
            return redirect(url_for('login'))
        return fn(**kwargs)
    return decorator


# Generate web-socket namespace from username in session
def socket_namespace_from_session(s):
    return "/" + urllib.parse.quote_plus(s['username'])


class ScriptThread:
    threads = {}

    def __init__(self, namespace, filename):
        self.aborted = False
        self.namespace = namespace
        self.kill_timer = None
        self.filename = filename
        ScriptThread.threads[namespace] = self
        self.task = socketio.start_background_task(target=self.run)

    def run(self):
        print('Started script thread')
        socketio.emit('script_started', namespace=self.namespace)
        program = [sys.executable, self.filename]
        with subprocess.Popen(program, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True) as proc:
            line = ''
            exiting = False
            while not exiting:
                char = proc.stdout.read(1)
                if char is None or char == '':
                    exiting = proc.poll() is not None
                else:
                    line += char
                if char == '\n' or exiting:
                    socketio.emit('script_output', escape(line) + Markup('<br/>'), namespace=self.namespace)
                    line = ''

                if self.aborted:
                    if self.kill_timer is None:
                        print('Aborting script thread')
                        proc.send_signal(signal.SIGINT)
                        self.kill_timer = time.monotonic() + 5
                    elif self.kill_timer < time.monotonic():
                        print('KILLING script thread')
                        proc.kill()
        ScriptThread.threads.pop(self.namespace)
        print('Finished script thread')
        socketio.emit('script_finished', proc.returncode, namespace=self.namespace)

    def abort(self):
        self.aborted = True


def start_script(script_code: str):
    namespace = socket_namespace_from_session(session)
    if namespace not in ScriptThread.threads:
        filename = f"{scripts_path}/{session['username']}.py"
        with open(filename, "w") as file:
            file.write(script_code)
        ScriptThread(namespace, filename)


def abort_script():
    namespace = socket_namespace_from_session(session)
    if namespace in ScriptThread.threads:
        ScriptThread.threads[namespace].abort()


@app.route("/")
@require_authentication
def home():
    user = session['username']
    filename = f"{scripts_path}/{user}.py"
    script_code = f'print("Hello {user}!")'
    if os.path.exists(filename):
        with open(filename, "r") as file:
            script_code = file.read()
    return render_template('home.html', namespace=socket_namespace_from_session(session), script_code=script_code)


@app.route('/login', methods=['GET', 'POST'])
def login():
    error = None
    if request.method == 'POST':
        username = request.form['username'].strip(' ')
        if len(username) > 0:
            session['username'] = username
            socketio.on_event('run_script', start_script, namespace=socket_namespace_from_session(session))
            socketio.on_event('abort_script', abort_script, namespace=socket_namespace_from_session(session))
            return redirect('/')
        else:
            error = "Missing username"
    return render_template('login.html', error=error)


@app.route('/logout')
def logout():
    session.pop('username', None)
    return redirect(url_for('login'))


if __name__ == '__main__':
    socketio.run(app, debug=True, host='0.0.0.0', port=5000)
