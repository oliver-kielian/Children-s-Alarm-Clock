// Source for how to get bluetooth up and running for devices in general
// ("https://medium.com/@kamresh485/exploring-the-web-bluetooth-api-use-cases-advantages-and-limitations-6f3f85946e44")

import { morningAlarmFunction } from "/static/morningAlarmScript.js";
import { morningLightFunction } from "/static/morningLightScript.js";

import { nightAlarmFunction } from "/static/nightAlarmScript.js";
import { nightLightFunction } from "/static/nightLightScript.js";

import { brightnessForLightsFunction } from "/static/brightnessForLights.js";
import { sendBrightnessForLightsFunction } from "/static/brightnessForLights.js";

import { manualLightFunction } from "/static/manualLightScript.js";

brightnessForLightsFunction();


let our_service;
let our_characteristic;
let gattCharacteristic;


fetch('static/config.json')
  .then(response => response.json())
  .then(config => {
    // console.log(config.OUR_SERVICE);
    // console.log(config.OUR_CHARACTERISTIC);
    our_service = config.OUR_SERVICE
    our_characteristic = config.OUR_CHARACTERISTIC
});

document.getElementById("saveSettingsButton").addEventListener("click", () =>{
    location.reload();
});


document.getElementById("bluetoothButton").addEventListener("click", () => {
    const statusElement = document.getElementById('status');

    navigator.bluetooth.requestDevice({
        filters: [{name: "Arduino"}],
        acceptAllDevices: false,
        optionalServices: [our_service]
    })
    .then(device => {
        statusElement.textContent = 'Connecting to ' + device.name + '...';
        return device.gatt.connect();
    })
    .then(server => {
        statusElement.textContent = 'Connected. Getting Arduino Service...';
        // console.log(server.getPrimaryService(our_service))
        return server.getPrimaryService(our_service);
    })
    .then(service => {
        statusElement.textContent = 'Getting Arduino Characteristic...';
        // console.log(service.getCharacteristic(our_characteristic))
        return service.getCharacteristic(our_characteristic);
    })
    .then(characteristic => {
        statusElement.textContent = 'Sucessfully Connected to Arduino...';
        console.log(characteristic);
        
        // console.log(our_service)
        // console.log(our_characteristic);
        gattCharacteristic = characteristic;

        morningLightFunction(gattCharacteristic);
        morningAlarmFunction(gattCharacteristic);

        nightLightFunction(gattCharacteristic);
        nightAlarmFunction(gattCharacteristic);
        
        sendBrightnessForLightsFunction(gattCharacteristic);

        manualLightFunction(gattCharacteristic);

        console.log(gattCharacteristic);
    })
    .catch(error => {
        statusElement.textContent = 'Error: ' + error;
    });
})
