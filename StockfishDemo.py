import module_manager
module_manager.review()
from stockfish import Stockfish
import chess
import random
import argparse
import sys
import serial
import time
import subprocess

class State:
    '''
    Game State Class
    Used to set, get, and update current game state

    Keeps track of current turn, last move made, current board fen, and game stockfish
    '''
    def __init__(self, fen: str, move: str):
        # self.turn = 1
        self.stockfish = Stockfish(path="/usr/games/stockfish")
        # print(type(chess.Board().fen()))
        self.stockfish.set_fen_position(fen)
        # TODO add updating to last move made
        self.lastMove = ''
        self.move = move
        self.fen = fen
        self.piece = self.Find_Piece_From_GameState()

    def Find_Piece_From_GameState(self):

        # Setup mappings
        rowToIndex = {1: 7, 2: 6, 3: 5, 4: 4, 5: 3, 6: 2, 7: 1, 8: 0}
        colToIndex = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'h': 7}

        move, fen = self.move, self.fen
        entireFen = fen.split(" ")
        boardFen = entireFen[0].split("/")
        src, dst = move[:2], move[2:]
        
        #get row from origin
        c, r = src[0], int(src[1])
        rowIndex = rowToIndex[r]
        colIndex = colToIndex[c]

        piece = ''
        curCol = 0
        for s in boardFen[rowIndex]:
            if s.isnumeric():
                curCol += int(s)
            elif curCol == colIndex:
                piece = s
                break
            else:
                curCol += 1
        return piece

def Make_Moves(moves, gameState: State):
    gameState.stockfish.make_moves_from_current_position(moves)
    gameState.fen = gameState.stockfish.get_fen_position()
    return gameState.fen


def Check_User_Move(gameState):
    '''
    Function to wait for, validate, and apply user input
    :param stockfish:
    :return:
    '''
    move = gameState.move
    if gameState.stockfish.is_move_correct(move):
        return Make_Moves([move], gameState)
        #print(gameState.stockfish.get_fen_position())
        # return gameState.stockfish.get_fen_position()
    else:
        return -1

def AI_Make_Move(gameState: State):
    '''
    Function to generate move for AI
    :param stockfish: Stockfish class representing current game being played
    :return: string move -> a string representing move being made by AI
    '''
    moves = gameState.stockfish.get_top_moves(3)
    random.shuffle(moves)
    move = moves[0]['Move']
    gameState.stockfish.make_moves_from_current_position([move])
    return move

def Get_Move_From_Serial():
    #establish arduino connection
    arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)
    time.sleep(0.1)
    if arduino.isOpen():
        while True:
            data = arduino.readline()
            print(data)
            #case on data being valid or invalid
            if data == '-1':
                continue
            else: 
                #make move with data
                break
    return



def Validate_State_Move(gameState: State):
    '''
    Start_Game()
    Function used to start a game of chess
    :return:
    '''
    
    if (gameState.stockfish.is_fen_valid(gameState.fen)):     # while our board is still valid - no stalemates, no checkmates, etc.
        gameState.fen = Check_User_Move(gameState)
        return gameState.fen
        # Pass_Move(gameState)
        #return error code
    else:
        # print("Not a valid FEN")
        return -1
    #print(gameState.stockfish.get_board_visual()) 
    


if __name__ == '__main__':
    #TODO: Make the arguments required
    parser = argparse.ArgumentParser(description='Validate Board State and Move Made')
    parser.add_argument('-move', metavar='M', type=str, nargs='?', required=True,
                        help='A move made by the user/opponent')
    parser.add_argument('-fen', metavar='F', type=str, nargs='?', required=True,
                        help='Current FEN state of game')
    parser.add_argument('-update', metavar='U', type=bool, default=False, nargs='?',
                        help='Just update the given FEN')

    args = parser.parse_args()
    move = args.move
    fen = args.fen
    update = args.update
    
    gameState = State(fen, move)
    
    result = "-1"
    if(update):
        result = str(Make_Moves([move], gameState))
    else:
        result = str(Validate_State_Move(gameState))

    #if(result == "-1"):
    #    gameState.fen = fen
            
    message = ""
    if(result == "-1"):
        message = "ERROR_MOVE_INVALID:" + gameState.piece + " " + gameState.move
    else:
        message = "USR_MOVE:"
        if(update):
            message = "OPP_MOVE:"
        message += gameState.piece + " " + gameState.move
    
    print(result)
    subprocess.run(['python3', 'audio_module.py', '-text', message]) # Call Audio module with respective message and wait for response
    
    
        # exit(0)
    



#if sys.argv[1] == 'Get_Move_From_User':
#    Get_Move_From_User(sys.argv[2])


#print(sys.argv[1])
