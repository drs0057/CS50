
document.addEventListener('DOMContentLoaded', function() {
    document.querySelector('form').addEventListener('submit', function(event) { // input to this function is the submit event
        let name = document.querySelector('#name').value; // #name will go to the DOM element that has the id "name"
        let utterance = new SpeechSynthesisUtterance(`hello, ${name}`);
        window.speechSynthesis.speak(utterance);
        event.preventDefault();
    });
});
