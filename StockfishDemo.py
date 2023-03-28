import module_manager
module_manager.review()
from stockfish import Stockfish
import chess
import random
import argparse
import sys
import serial
import time

class State:
    '''
    Game State Class
    Used to set, get, and update current game state

    Keeps track of current turn, last move made, current board fen, and game stockfish
    '''
    def __init__(self, fen: str, move: str):
        # self.turn = 1
        self.stockfish = Stockfish()
        # print(type(chess.Board().fen()))
        self.stockfish.set_fen_position(fen)
        # TODO add updating to last move made
        self.lastMove = ''
        self.move = move
        self.fen = fen

def Make_Moves(moves, gameState: State):
    gameState.stockfish.make_moves_from_current_position(moves)
    return gameState.stockfish.get_fen_position()


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

# def handleGameState(gameState: State):
#     '''
#     Function to handle game state changes

#     :param turn: integer value 0 or 1 representing current player's turn 
#     :param stockfish: Stockfish class representing current game being played

#     :return: turn, newFen -> return current player's turn after move is made or not made, updated fen of new board state
#     '''
#     # turn = gameState.fen.split(" ")[1]
#     # if turn == 'w':
#         #print(gameState.stockfish.get_board_visual())      # print current state of the board
#     move = Check_User_Move(gameState)     # request user move

#     # else:
#     #     move = AI_Make_Move(gameState)
#     #     print("Opponent made the following move: " + move)
#         # turn = 1
#     return gameState.stockfish.get_fen_position() # return new turn and update fen

# def Pass_Move(gameState):
#     turn = gameState.fen.split(" ")[1]
#     if(turn == 'w'):
#         return
#     return
def Get_Move_From_Serial():
    #establish arduino connection
    arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)
    while True:
        time.sleep(0.05)
        data = arduino.readline()
        #case on data being valid or invalid
        if data == '-1':
            continue
        else:
            #make move with data
            break

def Find_Piece_From_GameState(gameState):
    move, fen = gameState.move, gameState.fen
    rowToIndex = {1: 7, 2: 6, 3: 5, 4: 4, 5: 3, 6: 2, 7: 1, 8: 0}
    colToIndex = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'h': 7}
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
        print("Not a valid FEN")
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
    #print(move, fen, type(update))
    #print(fen)
    gameState = State(fen, move)
    #print(Find_Piece_From_GameState(gameState))
    result = "-1"
    if(update):
        result = Make_Moves([move], gameState)
        print(result)
        # exit(1)
    else:
        result = Validate_State_Move(gameState)
        print(result)
        # exit(0)
    



#if sys.argv[1] == 'Get_Move_From_User':
#    Get_Move_From_User(sys.argv[2])


#print(sys.argv[1])
