#----------------------------------------------------------------------------#
# Imports
#----------------------------------------------------------------------------#
from flask import Flask, render_template, request
# from flask.ext.sqlalchemy import SQLAlchemy
import logging
from logging import Formatter, FileHandler
from forms import *
import os

#----------------------------------------------------------------------------#
# App Config
#----------------------------------------------------------------------------#
app = Flask(__name__)
app.config.from_object('config')

#----------------------------------------------------------------------------#
# Controllers
#----------------------------------------------------------------------------#
@app.before_request
def log_request_info():
    print('Headers: %s', request.headers)
    print('Body: %s', request.get_data())

@app.route('/')
def home():
    return render_template('pages/placeholder.home.html')

@app.route('/api/mqtt/soil', methods=['POST'])
def forgot():
    if request.method == 'POST':
        print(request)
    return render_template('pages/placeholder.about.html')

# Error handlers
@app.errorhandler(500)
def internal_error(error):
    #db_session.rollback()
    return render_template('errors/500.html'), 500


@app.errorhandler(404)
def not_found_error(error):
    return render_template('errors/404.html'), 404

if not app.debug:
    file_handler = FileHandler('error.log')
    file_handler.setFormatter(
        Formatter('%(asctime)s %(levelname)s: %(message)s [in %(pathname)s:%(lineno)d]')
    )
    app.logger.setLevel(logging.INFO)
    file_handler.setLevel(logging.INFO)
    app.logger.addHandler(file_handler)
    app.logger.info('errors')

#----------------------------------------------------------------------------#
# Launch
#----------------------------------------------------------------------------#

# Default port:
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=4040)