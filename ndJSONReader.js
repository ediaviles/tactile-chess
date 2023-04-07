//import {eventController} from "./eventController";
//import {Chess} from "chess.js";
/*
const Chess = require('chess.js')
*/
//import Chess from "chess.js";
const axios = require('axios')
const {spawnSync, spawn} = require('child_process')
const ndjson = require('ndjson')
const EventEmitter = require('events')
const readline = require('readline');
const makeMove = require('./services.js')

const headers = {
    Authorization: 'Bearer ' + 'lip_Zt6rLGHWhZj8qcaeTaLG'
};

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
})

const prevData = null

const validatePythonProcess = (data) => {
        const dataString = data.toString().trim()
        if (dataString !== "-1" && dataString !== global.FEN) {
            global.FEN = dataString
            global.moves.push(dataJSON.move)
            axios.post(`https://lichess.org/api/board/game/${global.gameId}/move/${dataJSON.move}`, {}, {headers: headers})
            global.arduinoCommunication.stdout.off('data', handleArduinoMoveInfo)
        } else if (dataString === "-1") {console.log('invalid move')}
    }

const handleArduinoMoveInfo = (data) => {
        const dataJSONstrs = data.toString().trim().split('\n')
        const dataJSON = dataJSONstrs.reduce((acc, curr) => ({ ...acc, ...JSON.parse(curr) }), {});
        if (dataJSON !== prevData) {
            prevData = dataJSON
            const pythonProcess = spawn('python3', ['StockfishDemo.py', '-fen', global.FEN, '-move', dataJSON.move])
            pythonProcess.stdout.on('data', validatePython)
        }
    }

function getInfoFromArduino() {
    global.arduinoCommunication.on('data', handleArduinoMoveInfo)
}

async function ask(question) {
    rl.question(question, (answer) => {
        if(answer === "q") {
            process.exit(1)
        }
        //rl.write(`The answer received:  ${answer}\n`)
        //console.log('this is the game fen before move: '+global.FEN)
        //console.log('this is the move to be played: ' + answer)
        const pythonProcess = spawn('python3', ['StockfishDemo.py', '-fen', global.FEN, '-move', answer])
        pythonProcess.stdout.on('data', (data) => {
            //console.log('stdout: ' + data)
            const dataString = data.toString().trim()
            if (dataString !== "-1" && dataString !== global.FEN) {
                global.FEN = dataString
                global.moves.push(answer)
                axios.post(`https://lichess.org/api/board/game/${global.gameId}/move/${answer}`, {},
                {
                    headers: headers,
                    method: 'POST',
                    mode: 'cors'
                })
            } else if(dataString === "-1") {
                ask(question)
            }
        });
        pythonProcess.on('close', (code) => {
            //console.log('child process exited user move')
        })
    })
}


const eventController = async (data) => {
    switch(data.type) {
        case 'gameStart': {
            // once a game is started we want to open a new event stream to listen for chess moves being made
            //TODO
            global.gameId = data.game.fullId
            global.FEN = data.game.fen
            global.moves = []
            fetchData('stream game', {gameId: global.gameId})
            await ask('what is your move?')
            break;
        }
        case 'gameState': {
            let moves = data.moves
            if (moves !== '') {
                moves = moves.split(' ')
                lastMove = moves[moves.length - 1]
                if (global.moves !== null && !global.moves.includes(lastMove)) {
                    const pythonProcess = spawn('python3', ['StockfishDemo.py', '-fen', global.FEN, '-move', lastMove, '-update', 'True'])
                    pythonProcess.stdout.on('data', (data) => {
                        //console.log('stdout: '+ data)
                        const dataString = data.toString().trim()
                        if (dataString !== '-1' && dataString !== global.FEN) {
                            global.FEN = dataString
                        }
                    })
                    pythonProcess.on('close', (code) => {
                        //console.log('child process exited opponent move')
                        ask('what is your move?')
                    }); 
                }
            }
            break
        }
        case 'gameFinish': {
            // once a game is finished we close all streams and redirect user to home page
            //TODO
            break;
        }
        case 'makeMove': {
            const gameId = data.gameId
            const move = data.move
            axios.post(`https://lichess.org/api/board/game/${gameId}/move/${move}`, {},
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


const fetchData = async (command, props) => {
    const streamEmitter = new EventEmitter()
    switch (command) {
        case 'stream events': {
            let response = axios.get('https://lichess.org/api/stream/event',
                {responseType:"stream", headers: headers}
            ).then((response)=>{
                const stream = response.data.pipe(ndjson.parse());

                // Listen for the 'data' event and emit it on the streamEmitter instance
                stream.on('data', (data) => {
                    streamEmitter.emit('data', data);
                });

                // Listen for the 'end' event and emit it on the streamEmitter instance
                stream.on('end', () => {
                    streamEmitter.emit('end');
                    stream.destroy();
                });
            }).catch((error) => {
                    console.error(error);
            });
            break;
        }
        /*case 'game start': {
            gameId = data.gameId
            window.location.pathname = `/test/${gameId}`;
            break;
        }*/
        case 'stream game': {
            //gameId = props.gameId
            //console.log(gameState)
            let response = await axios.get(`https://lichess.org/api/board/game/stream/${global.gameId}`, {responseType:"stream", headers: headers}
            ).then((response)=>{
                const stream = response.data.pipe(ndjson.parse());

                // Listen for the 'data' event and emit it on the streamEmitter instance
                stream.on('data', (data) => {
                    streamEmitter.emit('data', data);
                });

                // Listen for the 'end' event and emit it on the streamEmitter instance
                stream.on('end', () => {
                    streamEmitter.emit('end');
                    stream.destroy()
                });
            }).catch((error) => {
                console.error(error);
            });
            break;
        }

        default:
            break;
    }
    streamEmitter.on('data', (data) => {
        if (data.hasOwnProperty('type')) {
            eventController(data, {})
        }
    });

    streamEmitter.on('end', () => {
        streamEmitter.removeAllListeners()
    });
    /*const reader = response.body.getReader();
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
                //TODO if we already started a game
                if (gameId !== null && turn === 'w') { // if we started a game and its users turn
                    console.log("starting subprocess")
                    const pythonProcess = spawnSync('python3', ['StockfishDemo.py', 'Get_Move_From_User', fen])
                    console.log(pythonProcess)
                }
                /!*if ("type" in newData && newData.type === "gameFull") {
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
                console.log(fen)*!/
                await eventController(newData)
            } catch (error) {
                console.error(error);
            }*//*
        }
    }*/
};

module.exports = fetchData;
