const buttons = document.querySelectorAll("button")

buttons.forEach(function (btn) {
    btn.addEventListener("click", function(e) {
        do_correction(e.target)
    });
});

function do_correction(clickedBtn) {
    // Vars
    const questionDiv = clickedBtn.parentNode;
    const dataType = questionDiv.getAttribute("data-type");
    const dataCorrect = questionDiv.getAttribute("data-correct");
    const answerDiv = questionDiv.querySelector(".answer");
    let isCorrectAnswer;

    // Check if question is already answered
    if(questionDiv.getAttribute("answered")) {
        return;
    }
    questionDiv.setAttribute("answered", true);

    // Check if user answered correctly
    if(dataType == "choice") { // "Multiple Choice" question
        isCorrectAnswer = clickedBtn.value == dataCorrect ? true : false;
    }
    else if(dataType == "text") { // "Free response" question
        const textInput = questionDiv.querySelector("input[type='text'")
        isCorrectAnswer = textInput.value == dataCorrect ? true : false;
    }

    // Display the answer (with specific styling wether the answer is correct or not)
    answerDiv.style.display = "block";
    if(isCorrectAnswer) {
        answerDiv.innerHTML = "<b>Correct!</b> " + answerDiv.innerHTML;
        answerDiv.classList.add("correct");
    } else {
        answerDiv.innerHTML = "<b>Incorrect.</b> " + answerDiv.innerHTML;
        answerDiv.classList.add("incorrect");
    }

    // Disactivate buttons and inputs
    questionDiv.querySelectorAll("button, input[type='text']").forEach(submit => {
        submit.classList.add("inactive")
    });
}