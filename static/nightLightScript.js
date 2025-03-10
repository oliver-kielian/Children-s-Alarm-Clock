const nightColorButton = document.getElementById('nightLight');
const selectedColorNight = document.getElementById('nightColor');
let sendColorIDNight = "";

// get id of color
selectedColorNight.addEventListener('input', function (){
    const color = this.value;
    console.log("Selected Color For Night:", color);
    sendColorIDNight = color;
});

nightColorButton.addEventListener('click', function (){
    fetch('/morningLight', {
        headers : {
            'Content-Type' : 'application/json'
        },
        method : 'POST',
        body : JSON.stringify( {
            // send here to our python then grab over there
            'color' : sendColorIDNight
        })
    })
    .then(function(response) {
        if(response.ok) {
            response.json()
            .then(function(response) {
                console.log(response);
            });
        }
        else {
            throw Error('Something went wrong');
        }
    })
    .catch(function(error) {
        console.log(error);
    });
});

