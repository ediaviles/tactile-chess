import RPi.GPIO as GPIO
import time
import subprocess

# Set up GPIO pin 17 as an input
GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.IN, pull_up_down=GPIO.PUD_UP)

def button_startGame(channel):
	print("Start game button pressed")
	process = subprocess.Popen(['node', 'services.js', 'createAISeek'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
	while True:
		user_input = input("Enter move: ")
		
		process.stdin.write(user_input.encode('utf-8') + b'\n')
		process.stdin.flush()
		
		output = process.stdout.readline().decode('utf-8').strip()
		print(output)
	print(output.decode())

def button_resignGame(channel):
	print("Resign game button pressed")

GPIO.add_event_detect(17, GPIO.FALLING, callback=button_startGame, bouncetime=200)

while True:
    pass
