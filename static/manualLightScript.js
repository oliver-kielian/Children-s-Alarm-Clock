import { utf8Encode } from "/static/functions.js";

export function manualLightFunction(gattCharacteristic){
    const manualColorButtonON = document.getElementById('manualLightON');
    const manualColorButtonOFF = document.getElementById('manualLightOFF');
    const selectedColorManual = document.getElementById('manualColor');
    let sendColorIDManual = "";


    selectedColorManual.addEventListener('input',  function (){
        const color = this.value;
        console.log("Selected Color For Manual Light:", color);
        sendColorIDManual = color;
    });
    
    manualColorButtonON.addEventListener('click', function (){
        if(!gattCharacteristic){
            return
        }
        const text = "LO";
        const encodedText = utf8Encode(text);
        const encodedColor = utf8Encode(sendColorIDManual);
        console.log(sendColorIDManual);
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
                'Data sent successfully Manual Light ON!!!\n\n' 
                + '▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n' 
            );
        })
        .catch(error => {
            console.error('Error sending data:', error);
        });     
    });

    manualColorButtonOFF.addEventListener('click', function (){
        if(!gattCharacteristic){
            return
        }
        const text = "LF";
        const encodedText = utf8Encode(text);
        console.log(encodedText);

        const combinedValues = new Uint8Array(encodedText.length);
        combinedValues.set(encodedText, 0);

        gattCharacteristic.writeValue(combinedValues)
        .then(() => {
            console.log('Data sent successfully!');
            alert(
                'Data sent successfully Manual Light OFF!!!\n\n' 
                + '▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n' 
            );
        })
        .catch(error => {
            console.error('Error sending data:', error);
        });     
    });
}

