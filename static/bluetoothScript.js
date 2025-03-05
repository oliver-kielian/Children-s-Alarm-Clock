// Source for how to get bluetooth up and running for devices in general
// ("https://medium.com/@kamresh485/exploring-the-web-bluetooth-api-use-cases-advantages-and-limitations-6f3f85946e44")
let ourDevice;

document.getElementById("bluetoothButton").addEventListener("click", () => {
    const statusElement = document.getElementById('status');

    navigator.bluetooth.requestDevice({
        acceptAllDevices: true,
        optionalServices: ['battery_service']
    })
    .then(device => {
        ourDevice = device
        statusElement.textContent = 'Connecting to ' + device.name + '...';
        fetch('/bluetooth', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({deviceName: device.name})
        })
            .then(response => {
                if(response.ok){
                    console.log("We have sent name successfuly!");
                } else{
                    console.error("We have failed to send name!")
                }
            })
            .catch(error =>{
                console.error("Error:", error)
            });
        return device.gatt.connect();
    })
    .then(server => {
        statusElement.textContent = 'Connected. Getting Battery Service...';
        return server.getPrimaryService('battery_service');
    })
    .then(service => {
        statusElement.textContent = 'Getting Battery Level Characteristic...';
        return service.getCharacteristic('battery_level');
    })
    .then(characteristic => {
        return characteristic.readValue();
    })
    .then(value => {
        const batteryLevel = value.getUint8(0);
        statusElement.textContent = 'Battery Level: ' + batteryLevel + '%';
    })
    .catch(error => {
        statusElement.textContent = 'Error: ' + error;
    });
})

// Disconnect from the device if we are not on the page
window.addEventListener('beforeunload', () =>{
    if(ourDevice && ourDevice.gatt.connected){
        ourDevice.gatt.disconnect();
        console.log("We have disconnected from arduino!");
    }
});