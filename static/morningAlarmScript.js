document.getElementById("morningAlarmButton").addEventListener("click", async () => {
    let timeValue = document.getElementById("morningAlarm").value;

    if (!timeValue) {
        document.getElementById("morningAlarmResponse").innerText = "Please select a time.";
        return;
    }

    try {
        let response = await fetch("/morningAlarm", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ time: timeValue })
        });

        let result = await response.json();
        document.getElementById("morningAlarmResponse").innerText = result.message;
    } catch (error) {
        document.getElementById("morningAlarmResponse").innerText = "Error setting alarm.";
        console.error("Fetch error:", error);
    }
});