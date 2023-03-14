//import { fetchData } from './ndJSONReader.js';
const fetchData = require('./ndJSONReader.js')
//import * as lib from './ndJSONReader.js';
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
    fetchData("stream events", null)

    //Then start a seek
    fetch(`https://lichess.org/api/challenge/ai`,
        {
            headers: headers,
            method: 'POST',
            mode: 'cors',
            body: formData({
                level: 1,
                'clock.limit': 60 * 3,
                'clock.increment': 2,
            })
        })
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

createAISeek()
