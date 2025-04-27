import { utf8Encode } from "/static/functions.js";

export function nightLightFunction(gattCharacteristic) {
    const nightColorButton = document.getElementById('nightLight');
    const selectedColorNight = document.getElementById('nightColor');
    let sendColorIDNight = "";

    selectedColorNight.addEventListener('input', function (){
        const color = this.value;
        console.log("Selected Color For Night:", color);
        sendColorIDNight = color;
    });

    nightColorButton.addEventListener('click', function (){
        if(!gattCharacteristic){
            console.log("Please connect to device first");
            return
        }
        const text = "N";
        const encodedText = utf8Encode(text);
        const encodedColor = utf8Encode(selectedColorNight);
        console.log(encodedColor);
        console.log(encodedText);

        const combinedValues = new Uint8Array(encodedText.length, encodedColor.length);
        combinedValues.set(encodedText, 0);
        combinedValues.set(encodedColor, encodedText.length);
        console.log(combinedValues);


        gattCharacteristic.writeValue(combinedValues)
        .then(() => {
            console.log('Data sent successfully!');
        })
        .catch(error => {
            console.error('Error sending data:', error);
        });     
    });
}




