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
    else{
         document.getElementById("nightAlarmResponse").innerText = "";
    }

    })

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
    else{
         document.getElementById("nightAlarmResponse").innerText = ""
    }
})