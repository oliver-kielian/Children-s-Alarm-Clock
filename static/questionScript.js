const colorQuestion = document.getElementById('colorQuestion');
const  lightQuestion = document.getElementById('lightQuestion');

colorQuestion.addEventListener('click', function(){
    alert("Customize the color for each of your alarms! When an alarm goes off, the clock will light up in the color you’ve chosen for that specific time. For example, set your 7 AM wake-up alarm to pink, and each morning at 7, your alarm clock will glow pink.");
})

lightQuestion.addEventListener('click', function(){
    alert("Set the time when you want your alarm clock to light up in your chosen color!")
})