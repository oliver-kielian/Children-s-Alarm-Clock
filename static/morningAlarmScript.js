import { utf8Encode } from "/static/functions.js";

export function morningAlarmFunction(gattCharacteristic) {
    document.getElementById("morningAlarmButton").addEventListener("click", async () => {
        let timeValue = document.getElementById("morningAlarm").value;
    
        if (!timeValue) {
            document.getElementById("morningAlarmResponse").innerText = "Please select a time.";
            return;
        }
    
        let [hours, minutes] = timeValue.split(":").map(Number);
    
        if(hours >= 12)
        {
            document.getElementById("morningAlarmResponse").innerText = "Please Select an AM time"
            return;
        }


        if(!gattCharacteristic){
            alert(
                'Error Sending Data!!\n\n' 
                + "\t• Please Make Sure You Connect To Alarm Before Sending Data\n" 
                + '▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n' 
            );
            return
        }
        if(hours < 10){
            hours = "0" + hours
        }
        console.log(hours)
        const text = "AM";
        const encodedText = utf8Encode(text);
        const encodedHours = utf8Encode(hours);
        const encodedMinutes = utf8Encode(minutes);

        console.log(encodedText);
        console.log(encodedHours);
        console.log(encodedMinutes);

        const combinedValues = new Uint8Array(encodedText.length + encodedHours.length + encodedMinutes.length);
        // console.log(combinedValues.length)

        combinedValues.set(encodedText, 0);
        // console.log(combinedValues);
        combinedValues.set(encodedHours, encodedText.length);
        // console.log(combinedValues);
        combinedValues.set(encodedMinutes, encodedHours.length + encodedText.length);
        console.log(combinedValues);


        gattCharacteristic.writeValue(combinedValues)
        .then(() => {
            console.log('Data Sent Successfully From Morning Alarm Script!');
        })
        .catch(error => {
            console.error('Error Sending Data From Morning Alarm Script:', error);
        });  
    });
}

