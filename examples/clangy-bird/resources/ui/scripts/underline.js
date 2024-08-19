"use strict";

function enterButton(button) {
    button.classList.add("underline");
}

function exitButton(button) {
    button.classList.remove("underline");
}

document.getNodesBySelector(".button").forEach((button) => {
    button.setAttribute("onenter", "enterButton(this)");
    button.setAttribute("onexit", "exitButton(this)");
});
