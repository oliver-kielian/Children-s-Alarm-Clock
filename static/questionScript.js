const colorQuestion = document.getElementById('colorQuestion');
const  lightQuestion = document.getElementById('lightQuestion');

colorQuestion.addEventListener('click', function(){
    alert("Choose the color you want for each alarm! At the selected alarm time, the color on the alarm clock with change to the color corresponding with the selected alarm (ex: For a wake up alarm (or morning alarm) set a 7 am with the color pink. At 7 am each day the lights on the alarm clock with shine pink)");
})

lightQuestion.addEventListener('click', function(){
    alert("Choose the time you want the lights on the alarm clock to change!")
})