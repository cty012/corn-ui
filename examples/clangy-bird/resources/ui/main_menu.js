function getGameTitle() {
    return document.getNodeBySelector("#title");
}

console.log("Hello world!");
console.log("0.1 + 0.2 = " + (0.1 + 0.2));
const gameTitle = getGameTitle();
gameTitle.text = "Injected by JavaScript";
console.log(gameTitle.text);
