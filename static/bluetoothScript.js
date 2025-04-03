// Source for how to get bluetooth up and running for devices in general
// ("https://medium.com/@kamresh485/exploring-the-web-bluetooth-api-use-cases-advantages-and-limitations-6f3f85946e44")

const our_service = '5f7325da-eca4-4d7d-ae15-7bd09b3d24f1';
const our_characteristic = '034da838-0810-44cb-ad23-8caa8d5ce1fe';
const morningColorButton = document.getElementById('morningLight');
const selectedColorMorning = document.getElementById('morningColor');
let sendColorIDMorning = "";

let gattCharacteristic;


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
        gattCharacteristic = characteristic;
        window.location.href = "lights.html";
        
    })
    .catch(error => {
        statusElement.textContent = 'Error: ' + error;
    });
})


// get id of color
selectedColorMorning.addEventListener('input', function (){
    const color = this.value;
    console.log("Selected Color For Morning:", color);
    sendColorIDMorning = color;
});

morningColorButton.addEventListener('click', function (){
    if(!gattCharacteristic){
        console.log("Please connect to device first");
        return
    }
    const text = "M";
    const encodedText = utf8Encode(text);
    console.log(encodedText)
    gattCharacteristic.writeValue(encodedText)
    .then(() => {
        console.log('Data sent successfully!');
    })
    .catch(error => {
        console.error('Error sending data:', error);
    });     
});

function utf8Encode(str) {
    return new TextEncoder().encode(str);
}

/*
// Disconnect from the device if we are not on the page
window.addEventListener('beforeunload', () =>{
    if(ourDevice && ourDevice.gatt.connected){
        ourDevice.gatt.disconnect();
        console.log("We have disconnected from arduino!");
    }
});
*/