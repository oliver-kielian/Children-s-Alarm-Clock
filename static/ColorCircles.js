const morningPicker = document.getElementById('morningColor');
const morningCircle = document.getElementById('morningColorCircle');

const nightPicker = document.getElementById('nightColor');
const nightCircle = document.getElementById('nightColorCircle')

morningPicker.addEventListener('input', function () {
    morningCircle.style.backgroundColor = morningPicker.value;
});

nightPicker.addEventListener('input', function(){
    nightCircle.style.backgroundColor = nightPicker.value;
});



