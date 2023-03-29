import module_manager
module_manager.review()
import pyttsx3

class Text_to_Speech_Engine(object):
    def __init__(self):
        self.engine = pyttsx3.init(driverName='espeak')
        # Set the properties of the voice
        self.engine.setProperty('rate', 70)    # Speed in words per minute
        self.engine.setProperty('volume', 0.7)  # Volume between 0 and 1

    # Define a callback function to reduce latency
    def callback(self, name, completed):
        # win32api.Sleep(0)
        pass

    def Vocalize(self, text: str):
        self.engine.say(text)
        self.engine.runAndWait()


if __name__ == '__main__':
    text_to_speech_engine = Text_to_Speech_Engine()
    
    # Vocalize the input string
    parser = argparse.ArgumentParser(description='Validate Board State and Move Made')
    parser.add_argument('-text', metavar='M', type=str, nargs='?', required=True,
                        help='Text to be vocalized by the script')

    args = parser.parse_args()
    text = args.text

    # Connect the callback function to the engine
    text_to_speech_engine.engine.connect('started-utterance', text_to_speech_engine.callback)
    text_to_speech_engine.Vocalize(text)