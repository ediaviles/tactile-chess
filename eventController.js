//import {fetchData} from "./ndJSONStreamReader";
const fetchData = require('./ndJSONReader.js')
const spawnSync = require('child_process')

const headers = {
    Authorization: 'Bearer ' + 'lip_Zt6rLGHWhZj8qcaeTaLG'
};

const eventController = async (data, gameState) => {
    switch(data.type) {
        case 'gameStart': {
            
            // once a game is started we want to open a new event stream to listen for chess moves being made
            //TODO
            //let gid = data.game.fullId;
            gameState.gameId = data.game.fullId
            gameState.fen = data.game.fen
            gameState.turn = data.game.color
            fetchData('stream game', {gameId: gid})
            const result = spawnSync('python3', ['StockfishDemo.py', 'Get_Move_From_User', [gameState.fen]])
            //console.log(result)
            break;
        }
        case 'gameFinish': {
            // once a game is finished we close all streams and redirect user to home page
            //TODO
            break;
        }
        case 'makeMove': {
            const gameId = data.gameId
            const move = data.move
            await fetch(`https://lichess.org/api/board/game/${gameId}/move/${move}`,
                {headers: headers,
                method: 'POST',
                mode: 'cors'})
            break;
        }
        case 'gameState': {

            break;
        }
        default: {
            // Treat default as a move made
            //TODO read the move and update that move onto the board
            break;
        }
    }
}

module.exports = eventController;