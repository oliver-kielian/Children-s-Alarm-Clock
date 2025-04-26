import { utf8Encode } from "/static/functions.js";

export function nightLightFunction(gattCharacteristic) {
    const nightColorButton = document.getElementById('nightLight');
    const selectedColorNight = document.getElementById('nightColor');
    const nightColorCircle = document.getElementById('nightColorCircle')
    let sendColorIDNight = "";

    selectedColorNight.addEventListener('input', function (){
        const color = this.value;
        console.log("Selected Color For Night:", color);
        sendColorIDNight = color;
        nightColorCircle.style.backgroundColor = color;
    });

    nightColorButton.addEventListener('click', function (){
        if(!gattCharacteristic){
            alert(
                'Error Sending Data!!\n\n' 
                + "\t• Please Make Sure You Connect To Alarm Before Sending Data\n" 
                + '▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n' 
            );
            return
        }
        const text = "N";
        const encodedText = utf8Encode(text);
        const encodedColor = utf8Encode(sendColorIDNight);
        console.log(sendColorIDNight);
        console.log(encodedText);
        console.log(encodedColor);
        

        const combinedValues = new Uint8Array(encodedText.length + encodedColor.length);
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


