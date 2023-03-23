const fetchData = require('./ndJSONReader.js')
const axios = require('axios')
const {spawnSync, spawn} = require('child_process')
const readline = require('readline');


const headers = {
    Authorization: 'Bearer ' + 'lip_Zt6rLGHWhZj8qcaeTaLG'
};

const formData = (data) => {
    const formData = new FormData();
    for (const k of Object.keys(data)) formData.append(k, data[k]);
    return formData;
};

const createAISeek = () => {
    //First start a stream
    console.log("In createAISeek")
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
    //Then start a seek
    /*fetch(`https://lichess.org/api/challenge/ai`,
        {
            headers: headers,
            method: 'POST',
            mode: 'cors',
            body: formData({
                level: 1,
                'clock.limit': 60 * 3,
                'clock.increment': 2,
            })
        })*/
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

createAISeek()
module.exports = makeMove
//const pyScript = spawn('python3', ['StockfishDemo.py', 'test']);

/*pyScript.stdin.on('ready', () => {*/
/*    console.log('ready')*/
/*})*/
/*
pyScript.stdout.on('data', (data) => {
    console.log(`stdout: ${data}`);
});

pyScript.stderr.on('data', (data) => {
    console.error(`stderr: ${data}`);
});


pyScript.on('close', (code) => {
    console.log(`child process exited with code ${code}`);
}); */

//const rl = readline.createInterface({
//    input: process.stdin,
//    output: process.stdout,
//})
//
//function ask(question) {
//    rl.question(question, (answer) => {
//        if(answer === "q") {
//            process.exit(1)
//        }
//        rl.write(`The answer received:  ${answer}\n`)
//        const pyScript = spawn('python3', ['StockfishDemo.py', 'test', answer])
//        pyScript.stdout.on('data', (data) => {
//             console.log(`stdout: ${data}`)
//        })
//        rl.close()
//    })
//}
//
//ask('what is your move?')



