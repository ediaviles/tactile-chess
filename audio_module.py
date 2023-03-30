import module_manager
module_manager.review()
import pyttsx3
import argparse

class Text_to_Speech_Engine(object):
	def __init__(self):
		# self.engine = pyttsx3.init(driverName='espeak')
		self.engine = pyttsx3.init()
		
		# Set the properties of the voice
		self.engine.setProperty('rate', 150)    # Speed in words per minute
		self.engine.setProperty('volume', 0.7)  # Volume between 0 and 1
		self.engine.setProperty('voice', 'espeak') # set the synthesis method to "espeak"
		
		self.piece_map = {
			"P": "Pawn",
			"B": "Bishop",
			"N": "Knight",
			"R": "Rook",
			"K": "King",
			"Q": "Queen"
		}

	# Define a callback function to reduce latency
	def callback(self, name, completed):
		# win32api.Sleep(0)
		pass

	def Find_Piece_Type(self, piece: str):
		color = ""
		if(piece.isupper()):
			color = "White"
		else:
			color = "Black"
			
		return f"{color} {self.piece_map[piece.upper()]}"
			
		

	def Vocalize(self, text: str):
	
		msg, msg_info = text.split(":")[0], text.split(":")[1]
		piece, move = self.Find_Piece_Type(msg_info.split(" ")[0]), msg_info.split(" ")[1]
		src, dst = move[:2], move[2:]
		if(text.startswith("ERROR")):
			message = f"The move {move} is invalid. Please move the {piece} back from {dst} to {src}."
			
		elif(text.startswith("OPP")):
			message = f"The opponent's move was {move}. Please move the {piece} from {src} to {dst}"
			
		elif(text.startswith("USR")):
			message = f"Your move is valid! Please wait for opponent's move."
			
				
		
		self.engine.say(message)
		self.engine.runAndWait()


if __name__ == '__main__':
	text_to_speech_engine = Text_to_Speech_Engine()

	# Vocalize the input string
	parser = argparse.ArgumentParser(description='Vocalize Board State and Move Made')
	parser.add_argument('-text', metavar='M', type=str, nargs='?', required=True,
						help='Text to be vocalized by the script')

	args = parser.parse_args()
	text = args.text

	# Connect the callback function to the engine
	# text_to_speech_engine.engine.connect('started-utterance', text_to_speech_engine.callback)
	text_to_speech_engine.Vocalize(text)
