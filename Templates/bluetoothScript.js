// Source for how to get bluetooth up and running for devices in general
// ("https://medium.com/@kamresh485/exploring-the-web-bluetooth-api-use-cases-advantages-and-limitations-6f3f85946e44")

document.getElementById("bluetoothButton").addEventListener("click", () => {
    const statusElement = document.getElementById('status');

    navigator.bluetooth.requestDevice({
        acceptAllDevices: true,
        optionalServices: ['battery_service']
    })
    .then(device => {
        statusElement.textContent = 'Connecting to ' + device.name + '...';
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