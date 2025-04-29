import { utf8Encode } from "/static/functions.js";
const sliderEl = document.querySelector("#range");
const sliderValue = document.querySelector(".value");


export function brightnessForLightsFunction() {
    sliderEl.addEventListener("input", (event) => {
    const tempSliderValue = event.target.value; 
    
    sliderValue.textContent = tempSliderValue;
    
    const progress = (tempSliderValue / sliderEl.max) * 100;
    
    sliderEl.style.background = `linear-gradient(to right, #f50 ${progress}%, #ccc ${progress}%)`;
    })
}

export function sendBrightnessForLightsFunction(gattCharacteristic) {
    const brightnessButton = document.getElementById('submitBrightness');

    brightnessButton.addEventListener('click', function (){
        if(!gattCharacteristic){
            // alert(
            //     'Error Sending Data!!\n\n' 
            //     + "\t• Please Make Sure You Connect To Alarm Before Sending Data\n" 
            //     + '▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n' 
            // );
            console.log("It worked")
            return
        }

        const text = "B";
        const encodedText = utf8Encode(text);
        const encodedSliderVal = utf8Encode(sliderValue);
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
