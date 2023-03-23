import module_manager
module_manager.review()
from stockfish import Stockfish
import chess
import random
import argparse

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

def Get_Move_From_User(gameState: State):
    '''
    Function to wait for, validate, and apply user input
    :param stockfish:
    :return:
    '''
    move = gameState.move
    if gameState.stockfish.is_move_correct(move):
        Make_Moves([move], gameState)
    else:
        print("Move is invalid, please try again!")

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

def handleGameState(gameState: State):
    '''
    Function to handle game state changes

    :param turn: integer value 0 or 1 representing current player's turn 
    :param stockfish: Stockfish class representing current game being played

    :return: turn, newFen -> return current player's turn after move is made or not made, updated fen of new board state
    '''
    turn = gameState.fen.split(" ")[1]
    if turn == 'w':
        print(gameState.stockfish.get_board_visual())      # print current state of the board
        move = Get_Move_From_User(gameState)     # request user move
        turn = 0

    else:
        move = AI_Make_Move(gameState)
        print("Opponent made the following move: " + move)
        turn = 1
    return gameState.stockfish.get_fen_position() # return new turn and update fen

def Pass_Move(gameState):
    turn = gameState.fen.split(" ")[1]
    if(turn == 'w'): #send the move
        j
    elif(turn == 'b'): #wait for move -> move to next state, player's turn

        


def Validate_State_Move(gameState: State):
    '''
    Start_Game()
    Function used to start a game of chess
    :return:
    '''
    
    if (gameState.stockfish.is_fen_valid(gameState.fen)):     # while our board is still valid - no stalemates, no checkmates, etc.
        gameState.fen = handleGameState(gameState)
        Pass_Move(gameState)
    else:
        print("Not a valid FEN")
    print(gameState.stockfish.get_board_visual()) 
    return -1

if __name__ == '__main__':
    #TODO: Make the arguments required
    parser = argparse.ArgumentParser(description='Validate Board State and Move Made')
    parser.add_argument('-move', metavar='M', type=str, nargs='?',
                        help='A move made by the user/opponent')
    parser.add_argument('-fen', metavar='F', type=str, nargs='?',
                        help='Current FEN state of game')

    args = parser.parse_args()
    move = args.move
    fen = args.fen


    gameState = State(fen, move)
    Validate_State_Move(gameState)
