function getGameTitle() {
    return document.getNodeBySelector("#title");
}

function getGameStartButton() {
    return document.getNodeBySelector("#start");
}

console.log("Hello world!");
console.log("0.1 + 0.2 = " + (0.1 + 0.2));

const gameStartButton = getGameStartButton();

gameStartButton.classList.add("red");

console.log(gameStartButton.classList);
