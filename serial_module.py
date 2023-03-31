import serial
import time


class CreateAction:
    def __init__(self):
        self.data = [] #A list of tuples. First element is piece. Second element is coordinate
        self.isCastling = False #'True' if in the process of castling

    def isMoveDone(self):
        pieces = dict()
        for piece, coordinate in self.data:
            if piece in pieces:
                if (not self.isCastling) and ((piece == "K" and "R" in pieces or piece == "k" and "r" in pieces) or \
                    (piece == "R" and "K" in pieces and piece == "r" and "k" in pieces)):
                    # Castling
                    self.isCastling = True
                    break
                elif (self.isCastling) and ((piece == "K" and "K" in pieces and pieces["K"] == 1 and "R" in pieces and pieces["R"] == 2) or \
                      (piece == "k" and "k" in pieces and pieces["k"] == 1 and "r" in pieces and pieces["r"] == 2) or \
                      (piece == "R" and "R" in pieces and pieces["R"] == 1 and "k" in pieces and pieces["k"] == 2) or \
                      (piece == "r" and "r" in pieces and pieces["r"] == 1 and "k" in pieces and pieces["k"] == 2)):
                    #castling complete
                    break
                
                
            else:
                pieces.add(piece)




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

                #edge is if same color king and rook are picked up
                #if len(data) == 2 -> (same piece) = piece move, (different piece != same color king and rook) = capture
                # (different piece = same color king and rook) = castle
                
                action.isMoveDone()
            else:   
                # DATA IS INVALID AND ERROR HAS BEEN LOGGED
                pass  
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
        