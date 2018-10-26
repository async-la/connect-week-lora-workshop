// Create Express with Socket server
const express = require('express')
const app = express()
const server = require('http').Server(app)
const io = require('socket.io')(server)
const ttn = require('ttn')
const loraSerialization = require('lora-serialization')


/**
===========================================================================
Add TTN application id and access key
===========================================================================
*/

const appId = ''
const accessKey = ''

/**
===========================================================================
*/


ttn.data(appId, accessKey).then(client => {
  client.on('message', (deviceId, data) => {
    console.info('[INFO] Message: ', deviceId, data)

    if (!data.payload_raw) return

    // Decode payload using lora-serialization library
    // https://github.com/thesolarnomad/lora-serialization
    let json = loraSerialization.decoder.decode(
      data.payload_raw,
      [loraSerialization.decoder.temperature, loraSerialization.decoder.uint16],
      ['temperature', 'voltage']
    );
    json.voltage /= 100

    console.log("## My Payload! ", json)

    // Emit event to all sockets for FE
    io.sockets.emit('device', { id: deviceId, payload: data })
  })
  
  client.on('connect', connack => console.log('[DEBUG]', 'Connect:', connack))
  client.on('error', err => console.error('[ERROR]', err.message))
  client.on('activation', (deviceId, data) => console.log('[INFO] ', 'Activation:', deviceId, data))
  client.on('device', null, 'down/scheduled', (deviceId, data) => console.log('[INFO] ', 'Scheduled:', deviceId, data))
  
  // Close the TTN client on exit
  process.on('exit', (code) => {
    client.end()
  })
}).catch((err) => {
  console.error('[ERROR]', err)
})
