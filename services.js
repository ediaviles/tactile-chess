const fetchData = require('./ndJSONReader.js')
const axios = require('axios')
const {spawnSync, spawn} = require('child_process');
const { create } = require('domain');
const readline = require('readline');
const Gpio = require('onoff').Gpio;



const headers = {
    Authorization: 'Bearer ' + 'lip_Zt6rLGHWhZj8qcaeTaLG'
};

const formData = (data) => {
    const formData = new FormData();
    for (const k of Object.keys(data)) formData.append(k, data[k]);
    return formData;
};

const resignGame = (gameId) => {
    axios.post(`https://lichess.org/api/board/game/${gameId}/resign`,
        {},
        {headers: headers})
}

const createAISeek = () => {
    const data = {
        level: 1,
        'clock.limit': 60 * 3,
        'clock.increment': 2,
    }
    fetchData("stream events", null)
    axios.post(`https://lichess.org/api/challenge/ai`,
        {
            level: 1,
            color: 'black'
        },
        {headers: headers}
    )
}

const createOnlineSeek = () => {
    fetchData('stream events', null).then(r => {})
    fetch(`https://lichess.org/api/board/seek`,
        {
            headers: headers,
            method: 'POST',
            mode: 'cors',
            body: formData({
                rated: false,
                time: 180,
                increment: 30,
                variant: "standard",
                color: "white"
            })
        })
}

const makeMove = (gameId, move) => {
    fetch(`https://lichess.org/api/board/game/${gameId}/move/${move}`,
        {
            headers: headers,
            method: 'POST',
            mode: 'cors'
        })
}

//Estbalish GPIO pins and arduino communication

const GPIO_PIN_START = 17;
const GPIO_PIN_RESIGN = 16;
const GPIO_PIN_CONFIRM = 27; //confirm button
const gpio_start = new Gpio(GPIO_PIN_START, 'in', 'both')
const gpio_resign = new Gpio(GPIO_PIN_RESIGN, 'in', 'both')
const gpio_confirm = new Gpio(GPIO_PIN_CONFIRM, 'in', 'both')

global.gameId = null
global.gameFEN = null
global.moves = null
global.isCalibrationDone = false
global.arduinoCommunication = null

const listenForCalibration = (data) => {
                    const dataJSONstrs = data.toString().trim().split('\n')
                    const dataJSON = dataJSONstrs.reduce((acc, curr) => ({ ...acc, ...JSON.parse(curr) }), {});
                    console.log("listenForCalibration dataJSON: ")
                    console.log(dataJSON)
                    //console.log(dataJSON) // this should be the information thats being received
                    //TODO case on information and start game when a specific condition is met
                    if (dataJSON.hasOwnProperty("isCalibrationDone") && dataJSON.isCalibrationDone === true && global.gameId === null && global.isCalibrationDone === false) {
                        console.log('Calibration is done')
                        global.action = "boardSetup"
                        global.isConfirmState = true
                        global.minVoltage = dataJSON.actionResult.split(" ")[0]
                        global.maxVoltage = dataJSON.actionResult.split(" ")[1]
                        global.isCalibrationDone = true
                        global.arduinoCommunication.stdout.off('data', listenForCalibration)
                        const message = "GAME_START:Please arrange the board to the starting position, and press the 'Confirm' button to begin game"
                        const pythonProcess = spawn('python3', ['audio_module.py', '-text', message])

                        //console.log('Game seek started')
                        //createAISeek()
                        
                    }
                }
                
const listenForGameStart = (data) => {
                    const dataJSONstrs = data.toString().trim().split('\n')
                    const dataJSON = dataJSONstrs.reduce((acc, curr) => ({ ...acc, ...JSON.parse(curr) }), {});
                    console.log("listenForGameStart dataJSON: ")
                    console.log(dataJSON) // this should be the information thats being received
                    //TODO case on information and start game when a specific condition is met
                    if (dataJSON.hasOwnProperty("isCalibrationDone") && dataJSON.isCalibrationDone === true && 
                        dataJSON.hasOwnProperty("actionType") && dataJSON.actionType === "Begin Game" &&
                        global.gameId === null && global.isCalibrationDone === true) {
                        global.arduinoCommunication.stdout.off('data', listenForGameStart)
                        console.log('Game seek started')
                        createAISeek()
                    }
                }

const confirmAction = (action) => {
    switch(action) {
        case "calibrationStart":
            console.log("in calibration start case")
            global.arduinoCommunication = spawn('python3', ['serial_module.py', '-startCalibration', 'True'])
            global.arduinoCommunication.stdout.on('data', listenForCalibration)
            global.isConfirmState = false
            break;
        case "boardSetup":
            global.arduinoCommunication.kill('SIGTERM')
            console.log("in board setup confirmation case")
            global.arduinoCommunication = spawn('python3', ['serial_module.py', '-beginGame', 'True'])
            console.log("after spawning process")
            global.arduinoCommunication.stdout.on('data', listenForGameStart)
            global.isConfirmState = false
            break;
        case "resign":
            break;
        default:
            console.log("no action to confirm")
    }
}

function main() {
    global.gameId = null
    global.gameFEN = null
    global.moves = null
    global.isCalibrationDone = false
    global.isConfirmState = false
    global.action = ""
    global.arduinoCommunication = null
    global.color = ""
    
    gpio_start.watch((err, value) => {
        if (err) {
            throw err;
        }
        if (value === 1) {
            // calibration mode which calls the audio module
            // waits for confirm
            // once confirm is pressed the arduino runs
            if (global.arduinoCommunication !== null) {
                global.isCalibrationDone = false;
                global.arduinoCommunication.kill('SIGTERM');
            }
            //global.arduinoCommunication = spawn('python3', ['serial_module.py', '-startCalibration', 'True'])
            //spawn audio instead to start calibration state
            const message = "CAL_CHECK:Please remove all the pieces from the board and press the 'Confirm' button, to start calibration"
            const audioModule = spawn('python3', ['audio_module.py', '-text', message]) // notify the user we're moving to calibration step and to hit confirm
            global.isConfirmState = true
            global.action = "calibrationStart"
            console.log("start game button pressed")
            //global.arduinoCommunication.stdout.on('data', listenForCalibration)
        }
    })

    gpio_resign.watch((err, value) => {
        if (err) {
            throw err;
        }
        if (value === 1 && global.gameId !== null) {
            console.log('Game resigned')
            // Once we resign game reset all values related to the game state
            resignGame(global.gameId)

            //Reseting game state values
            global.arduinoCommunication.kill('SIGTERM')
            const message = "RES_GAME:You have resigned the game by pressing the 'Confirm' button. Press the 'Start Game' button to start a new game"
            const audioModule = spawn('python3', ['audio_module.py', '-text', message]) // notify the user we're moving to calibration step and to hit confirm
            global.gameId = null
            global.FEN = null
            global.moves = null
            global.isCalibrationDone = false
        }
    })

    gpio_confirm.watch((err, value) => {
        if (err) {
            throw err;
        }
        if (value === 1 && global.isConfirmState === true) {
            console.log("in confirm button press")
            confirmAction(global.action)
        }
    })
}

if (require.main === module) {
	main();
}
module.exports = makeMove
