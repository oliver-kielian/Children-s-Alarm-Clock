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
            return
        }

        const text = "PM";
        const encodedText = utf8Encode(text);
        const encodedHours = utf8Encode(hours);
        const encodedMinutes = utf8Encode(minutes);


        console.log(encodedText);
        console.log(encodedHours);
        console.log(encodedMinutes);

        const combinedValues = new Uint8Array(encodedText.length + encodedHours.length + encodedMinutes.length);
        combinedValues.set(encodedText, 0);
        combinedValues.set(encodedHours, encodedText.length);
        combinedValues.set(encodedMinutes, encodedHours.length + encodedText.length);
        console.log(combinedValues);

        gattCharacteristic.writeValue(combinedValues)
        .then(() => {
            console.log('Data Sent Successfully From Night Alarm Script!');
            alert(
                'Data sent successfully Night Alarm!!!\n\n' 
                + '▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n' 
            );
        })
        .catch(error => {
            console.error('Error Sending Data From Night Alarm Script:', error);
        });  
    });
}



