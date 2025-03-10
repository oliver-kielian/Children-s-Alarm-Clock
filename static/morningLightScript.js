const morningColorButton = document.getElementById('morningLight');
const selectedColorMorning = document.getElementById('morningColor');
let sendColorIDMorning = "";

// get id of color
selectedColorMorning.addEventListener('input', function (){
    const color = this.value;
    console.log("Selected Color For Morning:", color);
    sendColorIDMorning = color;
});

morningColorButton.addEventListener('click', function (){
    fetch('/morningLight', {
        headers : {
            'Content-Type' : 'application/json'
        },
        method : 'POST',
        body : JSON.stringify( {
            // send here to our python then grab over there
            'color' : sendColorIDMorning
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

