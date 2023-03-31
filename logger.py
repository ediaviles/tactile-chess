import argparse
import logging

class Log(object):
	def __init__(self):
		# Set up logging
		logging.basicConfig(filename='./error.log', level=logging.ERROR)



# Generate an error to log
try:
    a = 1 / 0
except Exception as e:
    # Log the error
    logging.error(str(e))



if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Vocalize Board State and Move Made')
	parser.add_argument('-text', metavar='M', type=str, nargs='?', required=True,
						help='Text to be vocalized by the script')

	args = parser.parse_args()
	text = args.text
