document.getElementById("nightAlarmButton").addEventListener("click", async () => {
    let timeValue = document.getElementById("nightAlarm").value;

    if (!timeValue) {
        document.getElementById("nightAlarmResponse").innerText = "Please select a time.";
        return;
    }

    try {
        let response = await fetch("/nightAlarm", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ time: timeValue })
        });

        let result = await response.json();
        document.getElementById("nightAlarmResponse").innerText = result.message;
    } catch (error) {
        document.getElementById("nightAlarmResponse").innerText = "Error setting alarm.";
        console.error("Fetch error:", error);
    }
});