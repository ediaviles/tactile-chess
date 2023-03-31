import serial
import time
import json

class CreateAction:
    def __init__(self):
        self.data = [] #A list of tuples. First element is piece. Second element is coordinate
        self.isCastling = False #'True' if in the process of castling
        self.actionType = ''
        
    def to_dict(self):
        return {
            'move': self.move,
            'isCastling': self.isCastling,
            'actionType': self.actionType,
            'actionResult': self.actionResult
        }

    def isMoveDone(self):
        pieces = dict()
        for piece, coordinate in self.data:
            if piece in pieces:
                if (not self.isCastling) and ((piece == "K" and "R" in pieces or piece == "k" and "r" in pieces) or \
                    (piece == "R" and "K" in pieces and piece == "r" and "k" in pieces)):
                    # Castling
                    self.isCastling = True
                elif (self.isCastling) and ((piece == "K" and "K" in pieces and pieces["K"][0] == 1 and "R" in pieces and pieces["R"][0] == 2) or \
                      (piece == "k" and "k" in pieces and pieces["k"][0] == 1 and "r" in pieces and pieces["r"][0] == 2) or \
                      (piece == "R" and "R" in pieces and pieces["R"][0] == 1 and "k" in pieces and pieces["k"][0] == 2) or \
                      (piece == "r" and "r" in pieces and pieces["r"][0] == 1 and "k" in pieces and pieces["k"][0] == 2)):
                    #castling complete
                    self.actionType = "Castling"
                    currPiece, currCoor = piece, pieces[piece][1]+coor
                    pieces.pop(piece)
                    resPiece, coorInfo = next(iter(my_dict.items()))
                    self.move = resPiece + coorInfo
                    self.actionResult = currPiece + currCoor
                    break
                elif(len(data) == 3):
                    self.actionType = "Capture"
                    self.move = piece + pieces.pop(piece)[1] + coordinate
                    resPiece, coor_info = next(iter(my_dict.items()))
                    self.actionResult = resPiece + coor_info[1]
                    break
                elif(len(data) == 2):
                    self.actionType = "Move"
                    self.move = piece + pieces.pop(piece)[1] + coordinate
                    self.actionResult = ''
                    break
                
                pieces[piece] = (pieces[piece][0] + 1, pieces[piece][1] + coordinate)
            else: 
                pieces[piece] = (1, coordinate)
                    
        if(self.actionType != ''):
            return True
        else:
            return False
            
    def Send_Action(self):
        pass



class Arduino:
    def __init__(self):
        self.arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)
        self.pieces = set({"P", "R", "N", "B", "Q", "K"
                           "p", "r", "n", "b", "q", "k"})

    def establishSerialCommunication(self):
        action = CreateAction()
        while True:
            data = self.arduino.readline()
            if(self.Validate_Data(data)):
                data_decoded = data.decode('utf_8', errors='ignore')
                piece, coordinate = data_decoded[0], data_decoded[1:]
                if((piece, coordinate) in action.data): #Piece is lifted and placed down
                    action.data.remove((piece, coordinate))
                else:
                    action.data.append((piece, coordinate))
                
                if(action.isMoveDone()):
                    # send action info over to JS script
                    action.Send_Action()
                    pass
                    
                
                
            else:
                message = f"Error on in data received from arduino. Data received was {data}"
                subprocess.Popen(['python3', 'logger.py', '-message', message])
        return
    
    def Validate_Data(self, data):
        '''
        Data from Arduino Uno has to be of the format "<piece type><column coordinate (letter)><row coordinate (number)
        '''
        data_decoded = data.decode('utf-8', errors='ignore')
        print(data_decoded)
        try:
            if(len(data_decoded) != 3):
                #Log Error
                pass
            if(data_decoded[0] not in self.pieces):
                #Log Error
                pass
            if(not data_decoded[1].isalpha()):
                #Log Error
                pass
            if(not data_decoded[2].isnum()):
                #Log Error
                pass
            return True
        except:
            #Data was not decoded properly, or is not a string after decoding
            return False

if __name__ == '__main__':
    arduino = Arduino()
    time.sleep(0.1)
    if arduino.arduino.isOpen():
        #Get data and validate it
        arduino.establishSerialCommunication()

        #Create a Move
        
