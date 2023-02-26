import module_manager
module_manager.review()
from stockfish import Stockfish
import chess
import random

class State:
    '''
    Game State Class
    Used to set, get, and update current game state

    Keeps track of current turn, last move made, current board fen, and game stockfish
    '''
    def __init__(self):
        self.turn = 1
        self.stockfish = Stockfish()
        self.stockfish.set_fen_position(chess.Board().fen())
        # TODO add updating to last move made
        self.lastMove = ''
        self.fen = chess.Board().fen()

    def updateGameState(self, newTurn, newFen, lastMove):
        self.turn = newTurn
        self.fen = newFen
        self.lastMove = lastMove

def Get_Move_From_User(stockfish: Stockfish):
    '''
    Function to wait for, validate, and apply user input
    :param stockfish:
    :return:
    '''
    while True:
        move = input("Enter your move:\n")
        if stockfish.is_move_correct(move):
            stockfish.make_moves_from_current_position([move])
            break;
        else:
            print("Move is invalid, please try again!")
    return move

def AI_Make_Move(stockfish: Stockfish):
    '''
    Function to generate move for AI
    :param stockfish: Stockfish class representing current game being played
    :return: string move -> a string representing move being made by AI
    '''
    moves = stockfish.get_top_moves(3)
    random.shuffle(moves)
    move = moves[0]['Move']
    stockfish.make_moves_from_current_position([move])
    return move

def handleGameState(turn, stockfish: Stockfish):
    '''
    Function to handle game state changes
    :param turn: integer value 0 or 1 representing current player's turn
    :param stockfish: Stockfish class representing current game being played
    :return: turn, newFen -> return current player's turn after move is made or not made, updated fen of new board state
    '''
    if turn:
        print(stockfish.get_board_visual())      # print current state of the board
        move = Get_Move_From_User(stockfish)     # request user move
        turn = 0
    else:
        move = AI_Make_Move(stockfish)
        print("Opponent made the following move: " + move)
        turn = 1
    return turn, stockfish.get_fen_position(), move # return new turn and update fen

def Start_Game():
    '''
    Start_Game()
    Function used to start a game of chess
    :return:
    '''
    gameState = State()
    while (gameState.stockfish.is_fen_valid(gameState.fen)):     # while our board is still valid - no stalemates, no checkmates, etc.
        newTurn, newFen, lastMove = handleGameState(gameState.turn, gameState.stockfish)
        gameState.updateGameState(newTurn, newFen, lastMove)
    return -1


Start_Game()
# print(stockfish.is_fen_valid("3NR3/n3kN2/1pQ1P1B1/1P4P1/P1p5/8/5P2/R3K3 b Q - 3 48")) example of checkmate returns fen is invalid or false
