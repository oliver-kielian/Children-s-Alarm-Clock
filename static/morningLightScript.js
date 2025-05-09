import { utf8Encode } from "/static/functions.js";

export function morningLightFunction(gattCharacteristic){
    const morningColorButton = document.getElementById('morningLight');
    const selectedColorMorning = document.getElementById('morningColor');
    let sendColorIDMorning = "";


    selectedColorMorning.addEventListener('input',  function (){
        const color = this.value;
        console.log("Selected Color For Morning:", color);
        sendColorIDMorning = color;
    });
    
    morningColorButton.addEventListener('click', function (){
        if(!gattCharacteristic){
            return
        }
        const text = "M";
        const encodedText = utf8Encode(text);
        const encodedColor = utf8Encode(sendColorIDMorning);
        console.log(sendColorIDMorning);
        console.log(encodedColor);
        console.log(encodedText);

        const combinedValues = new Uint8Array(encodedText.length + encodedColor.length);
        combinedValues.set(encodedText, 0);
        combinedValues.set(encodedColor, encodedText.length);
        console.log(combinedValues);

        gattCharacteristic.writeValue(combinedValues)
        .then(() => {
            console.log('Data sent successfully!');
            alert(
                'Data sent successfully Morning Light!!!\n\n' 
                + '▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n' 
            );
            fetch('/colors', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({slot: 'morning', color: sendColorIDMorning})
            });
        })
        .catch(error => {
            console.error('Error sending data:', error);
        });     
    });
}

