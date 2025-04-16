import { utf8Encode } from "/static/functions.js";

export function morningLightFunction(gattCharacteristic){
    const morningColorButton = document.getElementById('morningLight');
    const selectedColorMorning = document.getElementById('morningColor');
    const morningColorCircle = document.getElementById('morningColorCircle');
    let sendColorIDMorning = "";


    selectedColorMorning.addEventListener('input',  function (){
        const color = this.value;
        console.log("Selected Color For Morning:", color);
        sendColorIDMorning = color;
        morningColorCircle.style.backgroundColor = color
    });
    
    morningColorButton.addEventListener('click', function (){
        if(!gattCharacteristic){
            console.log("Please connect to device first");
            return
        }
        const text = "M";
        const encodedText = utf8Encode(text);
        const encodedColor = utf8Encode(selectedColorMorning);
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