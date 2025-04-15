import { utf8Encode } from "/static/functions.js";

export function nightAlarmFunction(gattCharacteristic) {
    document.getElementById("nightAlarmButton").addEventListener("click", async () => {
        let timeValue = document.getElementById("nightAlarm").value;
    
        if (!timeValue) {
            document.getElementById("nightAlarmResponse").innerText = "Please select a time.";
            return;
        }
    
        let [hours, minutes] = timeValue.split(":").map(Number);
    
        if(hours < 12)
            {
                document.getElementById("nightAlarmResponse").innerText = "Please Select a PM time"
                return;
            }
    
        console.log(timeValue)
        if(!gattCharacteristic){
            console.log("Please connect to device first");
            return
        }
        const text = "PM";
        const encodedText = utf8Encode(text);
        const encodedTime = utf8Encode(timeValue);

        console.log(encodedText)
        console.log(encodedTime)
        const combinedValues = new Uint8Array(encodedText.length + encodedTime.length);
        combinedValues.set(encodedText, 0);
        combinedValues.set(encodedTime, encodedText.length);
        console.log(combinedValues);
        gattCharacteristic.writeValue(combinedValues)
        .then(() => {
            console.log('Data Sent Successfully From Night Alarm Script!');
        })
        .catch(error => {
            console.error('Error Sending Data From Night Alarm Script:', error);
        });  
    });
}

