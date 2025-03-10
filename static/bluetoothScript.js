// Source for how to get bluetooth up and running for devices in general
// ("https://medium.com/@kamresh485/exploring-the-web-bluetooth-api-use-cases-advantages-and-limitations-6f3f85946e44")

const our_service = 'c4d6aa1d-e90b-42fa-977e-039e7b401994';
const our_characteristic = 'c4d6aa1d-e90b-42fa-977e-039e7b401994';

document.getElementById("bluetoothButton").addEventListener("click", () => {
    const statusElement = document.getElementById('status');

    navigator.bluetooth.requestDevice({
        filters: [{name: "Arduino"}],
        acceptAllDevices: false,
        optionalServices: [our_service]
    })
    .then(device => {
        statusElement.textContent = 'Connecting to ' + device.name + '...';
        fetch('/bluetooth', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({})
        }).then(response => {
            console.log('POST to /bluetooth done:', response.status);
        }).catch(error => {
            console.error('Error posting to /bluetooth:', error);
        });
        return device.gatt.connect();
    })
    .then(server => {
        statusElement.textContent = 'Connected. Getting our Service...';
        return server.getPrimaryService(our_service);
    })
    .then(service => {
        return service.getCharacteristic(our_characteristic);
    })
    .then(characteristic => {
        return characteristic.readValue();
    })
    .then(value => {
        const output = value.getUint8(0);
        statusElement.textContent = output;
    })
    .catch(error => {
        statusElement.textContent = 'Error: ' + error;
    });
})

