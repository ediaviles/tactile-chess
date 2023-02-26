import module_manager

module_manager.review()
from stockfish import Stockfish
from stockfish import StockfishException
import chess

board = chess.Board()


def Get_Move_From_User(stockfish: Stockfish, board: chess):
    print(stockfish.get_board_visual())
    move = input("Enter your move:\n")
    return move


def Setup(board: chess):
    stockfish = Stockfish()
    stockfish.set_depth(20)  # How deep the AI looks
    stockfish.set_skill_level(20)  # Highest rank stockfish
    stockfish.get_parameters()
    stockfish.set_fen_position(board.fen())
    while True:
        if (stockfish.is_fen_valid(board.fen())):
            move = Get_Move_From_User(stockfish, board)
            try:
                stockfish.make_moves_from_current_position([move])
            except Exception as e:
                print(e)


Setup(board)
