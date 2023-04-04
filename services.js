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
            color: 'white'
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
const GPIO_PIN_CONFIRM = 15;
const gpio_start = new Gpio(GPIO_PIN_START, 'in', 'both')
const gpio_resign = new Gpio(GPIO_PIN_RESIGN, 'in', 'both')
const gpio_confirm = new Gpio(GPIO_PIN_CONFIRM, 'in', 'both')
global.arduinoCommunication = spawn('python3', ['serial_module.py'])

function main() {
    gpio_start.watch((err, value) => {
        if (err) {
            throw err;
        }

        if (value === 1 && global.calibrating === false) {
            // calibration mode which calls the audio module
            // waits for confirm
            // once confirm is pressed the arduino runs
            console.log("Button pressed")
            global.calibrating = true
            global.arduinoCommunication.stdout.on('data', (data) => {
                const dataJSONstrs = data.trim().split('\n')
                const dataJSON = dataJSONstrs.map(dataJSONstr => {JSON.parse(dataJSONstr)})
                console.log(dataJSON) // this should be the information thats being received

                //TODO case on information and start game when a specific condition is met
                if (true && gameId === null) {
                    createAISeek()
                }
            })            
        }
    })

    gpio_resign.watch((err, value) => {
        if (err) {
            throw err;
        }

        if (value === 1 && global.gameId !== null) {
            // Once we resign game reset all values related to the game state
            resignGame(global.gameId)

            //Reseting game state values
            global.gameId = null
            global.FEN = null
            global.moves = null
            global.calibrating = false
        }
    })

    gpio_confirm.watch((err, value) => {
        if (err) {
            throw err;
        }

        if (value === 1 && global.calibrating === true) {
            console.log('confirm calibration')
        }
    })
}

if (require.main === module) {
	main();
}
module.exports = makeMove