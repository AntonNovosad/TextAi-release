document.addEventListener("DOMContentLoaded", () => {

const checkTextArea = document.getElementById("value");
const infoTextArea = document.getElementById("info");
const sendButton = document.getElementById("send");
const openButton = document.getElementById("open");

const serverIp = "185.242.106.39";
const regExp = /[^(\d+(.\d+)?)]/g;

const EMPTY_STRING = "";
const MIN_WORDS_COUNT = 3;
const MAX_PERCENTAGE = 100;
const CLASS_BTN_DISABLED = "btn_disabled";
const SEND_BTN_TITLE = `Enter at least ${MIN_WORDS_COUNT} words!`;

infoTextArea.disabled = true;
sendButton.classList.add(CLASS_BTN_DISABLED);
sendButton.title = SEND_BTN_TITLE;

checkTextArea.addEventListener("keyup", e => {
  const wordsArray = e.target.value.split(" ").filter( str => str.length > 0 );
  const wordsCounter = wordsArray.length;

  if (wordsCounter >= MIN_WORDS_COUNT) {
    sendButton.disabled = false;
    sendButton.classList.remove(CLASS_BTN_DISABLED);
    sendButton.title = EMPTY_STRING;
    
    if (e.code === "Enter") {
      sendButton.disabled = false;
      sendButton.classList.remove(CLASS_BTN_DISABLED);
      sendButton.title = EMPTY_STRING;
      sendButton.click();
    }
  } else {
    sendButton.disabled = true;
    sendButton.classList.add(CLASS_BTN_DISABLED);
    sendButton.title = SEND_BTN_TITLE;
  }
});

sendButton.addEventListener("click", async () => {
  const payload = "name=" + encodeURIComponent(checkTextArea.value);
  try {
    const response = await fetch(`http://${serverIp}:8080/cgi-bin/script.cgi`, {
        method: "POST",
        headers: {"Content-Type": "application/x-www-form-urlencoded"},
        body: payload
    });
    const text = await response.text();

    const answerNumber = MAX_PERCENTAGE - parseInt( text.replace(regExp, EMPTY_STRING) );
    document.querySelector("#opacity").classList.add("opacity");
    document.querySelector("#result").innerHTML = `${answerNumber}% unique`;
  } catch (e) {
    throw new Error(e.message);
  }
});

openButton.addEventListener("click", async () => {
  infoTextArea.innerText = EMPTY_STRING;
  try {
    const response = await fetch(`http://${serverIp}:8080/cgi-bin/text.cgi`);
    const text = await response.text();
    
    infoTextArea.innerHTML = text;
    infoTextArea.style.color = 'black';
  } catch (e) {
    throw new Error(e.message);
  }
});

});
