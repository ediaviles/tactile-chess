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

let prevData = null

const validatePythonProcess = (data) => {
        let dataString = data.toString().trim()
        console.log("in validatepythonprocess")
        console.log(dataString)
        if (dataString !== "-1" && dataString.split("$")[1] !== global.FEN) {
            dataString = dataString.split("$")
            global.FEN = dataString[1]
            global.message = dataString[2]
            let move = global.moves[global.moves.length - 1]
            axios.post(`https://lichess.org/api/board/game/${global.gameId}/move/${move}`, {}, {headers: headers})
            const audioModule = spawn('python3', ['audio_module.py', '-text', global.message])
            //global.arduinoCommunication.stdout.off('data', handleArduinoMoveInfo)
        } else if (dataString === "-1") {
            const invalidMove = global.moves.pop()
            const dst = invalidMove.slice(1)
            const src = invalidMove.slice(0, 2)
            console.log(`${invalidMove} is invalid`)
            global.message = `MSG:The move ${invalidMove} is invalid. Please move the piece back from ${dst} to ${src}.`
            const pythonProcess = spawn('python3', ['audio_module.py', 'text', global.message])
            //const message = ""
            //const pythonProcess = spawn('python3', ['audio_module.py', '-text', message])
        }
    }

const handleArduinoMoveInfo = (data) => {
        console.log(data.toString())
        const dataJSONstrs = data.toString().trim().split('\n')
        const dataJSON = dataJSONstrs.reduce((acc, curr) => ({ ...acc, ...JSON.parse(curr) }), {});
        let move_message = ""
        console.log(dataJSON)
        console.log("values")
        console.log(global.makeOpponentMove)
        console.log(dataJSON.move.slice(1))
        console.log(global.fixMove)
        console.log(global.opponentMove)
        console.log(dataJSON.move[0])
        console.log(global.opponentPiece)
        // Waiting for user to replicate opponent's online move on the physical board
        const dataString = data.toString().trim()
        if (global.makeOpponentMove === true && dataJSON.move.slice(1) === global.opponentMove && global.fixMove === "") { //&& dataJSON.move[0] === global.opponentPiece) {
            global.makeOpponentMove = false
            global.fixMove = ""
            prevData = dataJSON
            global.fixedMove = false
            global.message = "MKE_MVE:Please make your move"
            const pythonProcess = spawn('python3', ['audio_module.py', '-text', global.message])
        }
        else if (global.makeOpponentMove === true && dataJSON.move.slice(1) === global.fixMove) {
            global.fixMove = ""
            global.fixedMove = true
            const pythonProcess = spawn('python3', ['audio_module.py', '-text', global.message])
            global.fixMessage = ""
        }
        else if (global.makeOpponentMove === true && dataJSON.move.slice(1) !== global.opponentMove && dataJSON.move.slice(1) !== global.fixMove && global.moveFixed === false) {
            const move = dataJSON.move.slice(1)
            const dst = move.slice(2)
            const src = move.slice(0, 2)
            global.fixMessage = `MSG:The move was not replicated correctly, please move your piece back from ${dst} to ${src}, and replicate the move again`
            global.fixMove = dst + src
            const pythonProcess = spawn('python3', ['audio_module.py', '-text', global.fixMessage])
        }
        // Waiting for user to make their own move on the physical board
        else if (global.makeOpponentMove === false && (prevData === null || JSON.stringify(dataJSON) !== JSON.stringify(prevData))) {
        //if ((prevData === null || JSON.stringify(dataJSON) !== JSON.stringify(prevData))) {
            prevData = dataJSON
            global.moveFixed = false
            console.log("calling validatePythonPorcess on:")
            console.log(global.FEN, dataJSON.move.slice(1))
            global.moves.push(dataJSON.move.slice(1))
            const pythonProcess = spawn('python3', ['StockfishDemo.py', '-fen', global.FEN, '-move', dataJSON.move.slice(1)])
            pythonProcess.stdout.on('data', validatePythonProcess)
        }
    }

function getInfoFromArduino() {
    console.log("in get info from arduino")
    global.arduinoCommunication.stdout.on('data', handleArduinoMoveInfo)
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
            if (dataString !== "-1" && dataString.split("$")[1] !== global.FEN) {
                global.FEN = dataString.split("$")[1]
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


const eventController = (data) => {
    switch(data.type) {
        case 'gameStart': {
            // once a game is started we want to open a new event stream to listen for chess moves being made
            //TODO
            const message = "Game is Starting"
            const pythonProcess = spawn('python3', ['audio_module.py', '-text', message])
            global.gameId = data.game.fullId
            global.FEN = data.game.fen
            global.moves = []
            global.color = data.game.color
            //global.arduinoCommunication = spawn('python3', ['serial_module.py'])
            getInfoFromArduino()
            fetchData('stream game', {gameId: global.gameId})
            //await ask('what is your move?')
            console.log('before global message in gameStart')
            global.message = `MSG:The game has started, your piece color is ${global.color}`
            const audioModule = spawn('python3', ['audio_module.py', '-text', global.message])
            console.log('after spawn')
            if (global.color === 'white'){
                //getInfoFromArduino()
                global.message = "MKE_MVE:Please make your move"
                const pythonProcess = spawn('python3', ['audio_module.py', '-text', global.message])
            }
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
                        const dataString = data.toString().trim().split("$")
                        
                        if (dataString !== '-1' && dataString[1] !== global.FEN) {
                            global.FEN = dataString[1]
                            console.log("data string is: ", dataString)
                            console.log("global fen is: ")
                            console.log(global.FEN)
                            global.opponentMove = lastMove
                            global.opponentPiece = dataString[0]
                            global.makeOpponentMove = false
                            global.message = dataString[2]
                            const audioModule = spawn('python3', ['audio_module.py', '-text', global.message])
                            //console.log('Before getInfoFromArduino')
                            //getInfoFromArduino()
                        }
                    })
                    pythonProcess.on('close', (code) => {
                        //console.log('child process exited opponent move')
                        //ask('what is your move?')
                        
                    }); 
                }
            }
            break
        }
        case 'gameFinish': {
            // once a game is finished we close all streams and redirect user to home page
            //TODO
            //global.arduinoCommunication.kill('SI
            
            if (global.arduinoCommunication !== null) {
                global.arduinoCommunication.stdout.off('data', handleArduinoMoveInfo)
                global.arduinoCommunication.kill('SIGTERM')
            }
            global.gameId = null
            global.FEN = null
            global.moves = null
            global.isCalibrationDone = false
            global.arduinoCommunication = null
            global.message = `MSG:Game has finished ${data.game.winner} has won`
            const audioModule = spawn('python3', ['audio_module.py', '-text', global.message])
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
};

module.exports = fetchData;
