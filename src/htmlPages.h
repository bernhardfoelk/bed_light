#pragma once

#include <Arduino.h>

const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Bed Light</title>
<style>
  body {
      font-family: monospace;
      background: #121212; 
      color: #f5f5f7;
      display: flex;
      justify-content: center;
      align-items: flex-start; 
      min-height: 100vh;
      margin: 0;
  }

  #container {
      width: 90%;
      max-width: 300px;
      padding: 1.2rem;
      text-align: center;
      background: #2a2a2e;
      border-radius: 16px;
      box-shadow: 0 6px 15px rgba(0,0,0,0.3);
      margin-top: 40px; 
  }
  h1 {
    font-size: 1.8rem; 
    font-weight: 600;
    margin-bottom: 1.2rem;
  }
  h2 {
  font-size: 1.1rem;
  font-weight: 600;
  margin: 0.8rem 0;
}

  .rgb-slider {
    -webkit-appearance: none;
    width: 100%;
    height: 6px;
    border-radius: 3px;
    background: #3a3a3c; 
    outline: none;
    margin: 0.8rem 0;
  }
  .rgb-slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    width: 14px;
    height: 14px;
    border-radius: 50%;
    background: #9b9c9cff;
    border: 1px solid #f5f5f7;
    cursor: pointer;
  }
  .rgb-slider::-moz-range-thumb {
    width: 14px;
    height: 14px;
    border-radius: 50%;
    background: #9b9c9cff;
    border: 1px solid #f5f5f7;
    cursor: pointer;
  }

  /* Helligkeit & Timer Slider */
  .slider {
    -webkit-appearance: none;
    width: 100%;
    height: 6px;
    border-radius: 3px;
    background: #3a3a3c;
    outline: none;
    margin: 0.8rem 0;
  }
  .slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    width: 12px;
    height: 12px;
    border-radius: 50%;
    background: #9b9c9cff;
    border: 1px solid #f5f5f7;
    cursor: pointer;
  }
  .slider::-moz-range-thumb {
    width: 12px;
    height: 12px;
    border-radius: 50%;
    background: #9b9c9cff;
    border: 1px solid #f5f5f7;
    cursor: pointer;
  }

  /* Buttons */
  .button {
    display: block;
    width: 100%;
    padding: 0.6rem;
    margin: 0.4rem 0;
    font-size: 0.95rem;
    font-weight: 500;
    border-radius: 12px;
    border: 1px solid #3a3a3c;
    background: rgba(58, 58, 60, 0.6);
    color: #f5f5f7;
    transition: background 0.2s ease, transform 0.1s ease;
  }
  .button:hover { background: rgba(72, 72, 74, 0.8);}
  .button:active { transform: scale(0.97); }

  hr { margin: 1.2rem 0; border: none; border-top:1px solid #3a3a3c; }
  .status { margin-top:1rem; font-size:0.85rem; color:#a1a1a6; }
</style>
</head>
<body>
<div id="container">
  <h1>Bed Light</h1>

  <h2>Brightness: <span id="actualBrightness"></span>%</h2>
  <input type="range" min="0" max="100" value="40" class="slider" id="brightness">

  <button onclick="On()" id="OnButton" class="button">On</button>
  <button onclick="Off()" id="OffButton" class="button">Off</button>

  <hr>

  <h2>RGB Value</h2>
  <input type="range" min="0" max="255" value="123" class="rgb-slider" id="r">
  <input type="range" min="0" max="255" value="231" class="rgb-slider" id="g">
  <input type="range" min="0" max="255" value="223" class="rgb-slider" id="b">

  <hr>

  <h2>Sleep timer: <span id="actualTime"></span> min</h2>
  <input type="range" min="0" max="60" value="0" class="slider" id="time">

  <p class="status" id="status"></p>
</div>

<script>
function sendCommand(params) {
  fetch(`/FadeInOut?${params}`)
    .then(response=>response.text())
    .then(data=>{document.getElementById("status").innerText=data;});
}

function On(){ sendCommand(`brightness=${brightness.value}&r=${r.value}&g=${g.value}&b=${b.value}&time=${time.value}`); }
function Off(){ sendCommand("brightness=0&r=0&g=0&b=0&time=0"); }
function Personal(){ sendCommand(`brightness=${brightness.value}&r=${r.value}&g=${g.value}&b=${b.value}&time=${time.value}`); }
function Timer(){ sendCommand(`brightness=${brightness.value}&r=${r.value}&g=${g.value}&b=${b.value}&time=${time.value}`); }

function changeColor(){
  let red=r.value,green=g.value,blue=b.value;
  let color=`rgb(${red},${green},${blue})`;
  document.getElementById("OnButton").style.backgroundColor=color;
  /*document.getElementById("OffButton").style.backgroundColor=color;*/
}

r.addEventListener("input",changeColor);
g.addEventListener("input",changeColor);
b.addEventListener("input",changeColor);

function changeTime(){ actualTime.innerText=time.value; }
function changeBrightness(){ actualBrightness.innerText=brightness.value; }

time.addEventListener("input",changeTime);
brightness.addEventListener("input",changeBrightness);

changeTime();
changeBrightness();
changeColor();
document.getElementById("status").innerText = "Bereit";
</script>
</body>
</html>
)rawliteral";

