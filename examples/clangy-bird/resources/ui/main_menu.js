function enterButton(button) {
    button.classList.add("underline");
}

function exitButton(button) {
    button.classList.remove("underline");
}

document.getNodesBySelector(".button").forEach(function (button) {
    button.setAttribute("onmouseenter", "enterButton(this)");
    button.setAttribute("onmouseexit", "exitButton(this)");
});
