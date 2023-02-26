import module_manager
module_manager.review()
from stockfish import Stockfish
import chess
import random

def Get_Move_From_User(stockfish: Stockfish):
    move = input("Enter your move:\n")
    return move

def AI_Make_Move(stockfish: Stockfish):
    moves = stockfish.get_top_moves(3)
    random.shuffle(moves)
    move = moves[0]['Move']
    stockfish.make_moves_from_current_position([move])
    return move

def Start_Game():
    board = chess.Board()
    stockfish = Stockfish()
    stockfish.set_fen_position(board.fen())
    stockfish.set_depth(20)  # How deep the AI looks
    stockfish.set_skill_level(20)  # Highest rank stockfish
    turn = 1                                         # 1 is for white's turn 0 is for black's turn
    #Game Loop
    while (stockfish.is_fen_valid(stockfish.get_fen_position())):     # while our board is still valid - no stalemates, no checkmates, etc.
        if turn:
            print(stockfish.get_board_visual())      # print current state of the board
            move = Get_Move_From_User(stockfish)     # request user move
            if stockfish.is_move_correct(move):
                stockfish.make_moves_from_current_position([move])
                turn = 0
            else:                                    # invalid move
                print("Move is invalid, please try again")
        else:
            move = AI_Make_Move(stockfish)
            print("Opponent made the following move: " + move)
            turn = 1
    return -1





Start_Game()
# print(stockfish.is_fen_valid("3NR3/n3kN2/1pQ1P1B1/1P4P1/P1p5/8/5P2/R3K3 b Q - 3 48")) example of checkmate returns fen is invalid or false
