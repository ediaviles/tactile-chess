import argparse
import logging

class Log(object):
	def __init__(self, filename):
		# Set up logging
		self.logger = logging.getLogger(__name__)
		logging.basicConfig(filename=filename, level=logging.ERROR)
	
	def write_error(self, text):
		logging.error(str(text))


# # Generate an error to log
# try:
    # a = 1 / 0
# except Exception as e:
    # # Log the error
    # logging.error(str(e))



if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Vocalize Board State and Move Made')
	parser.add_argument('-text', metavar='M', type=str, nargs='?', required=True,
						help='Text to be vocalized by the script')
	parser.add_argument('-filename', metavar='F', type=str, nargs='?', required=True,
						help='Filename to log erros into')

	args = parser.parse_args()
	text = args.text
	filename = args.filename
	
	logger = Log(filename)
