import { utf8Encode } from "/static/functions.js";
const sliderEl = document.querySelector("#range");
const sliderValue = document.querySelector(".value");
var value = 0;


export function brightnessForLightsFunction() {
    sliderEl.addEventListener("input", (event) => {
    const tempSliderValue = event.target.value; 
    
    sliderValue.textContent = tempSliderValue;
    
    const progress = tempSliderValue;
    value = progress;
    sliderEl.style.background = `linear-gradient(to right, #f50 ${progress}%, #ccc ${progress}%)`;
    })
    
}

export function sendBrightnessForLightsFunction(gattCharacteristic) {
    const brightnessButton = document.getElementById('submitBrightness');

    brightnessButton.addEventListener('click', function (){
        if(!gattCharacteristic){
            return
        }
        
        const text = "B";
        const encodedText = utf8Encode(text);
        const encodedSliderVal = utf8Encode(value);
        console.log(encodedText);
        console.log(encodedSliderVal);
        

        const combinedValues = new Uint8Array(encodedText.length + encodedSliderVal.length);
        combinedValues.set(encodedText, 0);
        combinedValues.set(encodedSliderVal, encodedText.length);
        console.log(combinedValues);


        gattCharacteristic.writeValue(combinedValues)
        .then(() => {
            console.log('Data sent successfully!');
            alert(
                'Data sent successfully!!!\n\n' 
                + "\t• It worked!\n" 
                + '▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n' 
            );
        })
        .catch(error => {
            console.error('Error sending data:', error);
        });     
    });
  
}
