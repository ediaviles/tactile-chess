import unittest
import StockfishDemo
from stockfish import Stockfish
import chess

class MyTestCase(unittest.TestCase):
    def test_Checkmate_Detection(self):
        '''
        Test for when fen is in checkmate state
        '''
        gameState = StockfishDemo.State()
        gameState.fen = "3NR3/n3kN2/1pQ1P1B1/1P4P1/P1p5/8/5P2/R3K3 b Q - 3 48"
        assert(StockfishDemo.Start_Game(gameState) == -1)

    def test_Expect_Fen_After_Moves(self):
        '''
        Expect stockfish to update state based on some moves
        '''
        moves = ['e2e4', 'd7d5', 'b1c3', 'd5d4', 'c3d5', 'g8f6']
        stockfish = Stockfish()
        stockfish.set_fen_position(chess.Board().fen())
        expectedFen = "rnbqkb1r/ppp1pppp/5n2/3N4/3pP3/8/PPPP1PPP/R1BQKBNR w KQkq - 2 4"
        StockfishDemo.Make_Moves(moves, stockfish)
        assert(stockfish.get_fen_position() == expectedFen)

if __name__ == '__main__':
    unittest.main()
