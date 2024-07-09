Event.listen("listen::local", function (payload) {
    document.getNodeBySelector("#local-event").text = payload;
});

GlobalEvent.listen("listen::global", function (payload) {
    document.getNodeBySelector("#global-event").text = payload;
});

Event.emit("emit::local", "Local event emitted by JavaScript");
GlobalEvent.emit("emit::global", "Global event emitted by JavaScript");
