//import {eventController} from "./eventController";
//import {Chess} from "chess.js";
/*const Chess = require('chess.js')*/
//import Chess from "chess.js";
const axios = require('axios')

const headers = {
    Authorization: 'Bearer ' + 'lip_Zt6rLGHWhZj8qcaeTaLG'
};

const eventController = async (data) => {
    switch(data.type) {
        case 'gameStart': {
            
            // once a game is started we want to open a new event stream to listen for chess moves being made
            //TODO
            let gid = data.game.fullId;
            console.log(gid)
            fetchData('stream game', {gameId: gid})
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
        default: {
            // Treat default as a move made
            //TODO read the move and update that move onto the board
            break;
        }
    }
}
/*const chess = new Chess();
let movesMadeArr = []
let movesIndex = 0
let fen = ''*/
const fetchData = async (command, data) => {
    let response = null;
    switch (command) {
        case 'stream events': {
            response = await axios.get('https://lichess.org/api/stream/event',
                {headers: headers}
            )
            break;
        }
        case 'game start': {
            const gameId = data.gameId
            window.location.pathname = `/test/${gameId}`;
            break;
        }
        case 'stream game': {
            const gameId = data.gameId
            response = await fetch(`https://lichess.org/api/board/game/stream/${gameId}`, {
                headers: headers,
                method: 'GET',
                mode: 'cors'
            })
            break;
        }

        default:
            break;
    }
    const reader = response.body.getReader();
    let decoder = new TextDecoder('utf-8');
    let partialData = '';
    while (true) {
        const { done, value } = await reader.read();
        if (done) {
            break;
        }

        partialData += decoder.decode(value);

        // Split the partial data on newlines to separate NDJSON messages
        const messages = partialData.split('\n');

        // Keep the last incomplete message for the next iteration
        partialData = messages.pop();

        // Parse and add the complete messages to the component state
        for (const message of messages) {
            try {
                const newData = JSON.parse(message);
                console.log(newData)
                /*if ("type" in newData && newData.type === "gameFull") {
                    //TODO if we see a gameFull update our liveState
                    //first get array of moves
                    const movesMade = newData.state.moves.split(" ");
                    if (movesMade[0] !== '') {
                        for (let i = 0; i < movesMade.length; i++) {
                            chess.move(movesMade[i])
                        }
                        //data.setMovesMade(movesMade)
                        //data.setMovesIndex(movesMade.length)
                        movesMadeArr = movesMade
                        movesIndex = movesMade.length
                    } else {
                        //data.setMovesMade([])
                        //data.setMovesIndex(0)
                    }
                    fen = chess.fen(); // this is our live fen
                }
                else if ("type" in newData && newData.type === "gameState") {
                    //TODO update our moves made
                    const movesMade = newData.state.moves.split(" ")
                    //data.setMovesMade(newData.moves.split(" "))
                    for (let i = movesIndex; i < movesMade.length; i++) {
                        chess.move(movesMade[i])
                    }
                    movesMadeArr = movesMade;
                    movesIndex = movesMade.length;
                    fen = chess.fen();
                }
                console.log(fen)*/
                await eventController(newData)
            } catch (error) {
                console.error(error);
            }
        }
    }
};

module.exports = fetchData;