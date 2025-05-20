static const char MAIN_PAGE[] PROGMEM = R"=====(
  <!-- index.html -->
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
    <title>Image to PDF Converter</title>
    <style>
    .arrows {
      font-size:40px;
      color:black;
    }
    td.button {
      background-color:white;
      border-radius:5%;
      border-style: solid;
      text-align: center;
    }
    td.button:active {
      transform: translate(5px,5px);
      box-shadow: none; 
    }

    .noselect {
      -webkit-touch-callout: none; /* iOS Safari */
        -webkit-user-select: none; /* Safari */
         -khtml-user-select: none; /* Konqueror HTML */
           -moz-user-select: none; /* Firefox */
            -ms-user-select: none; /* Internet Explorer/Edge */
                user-select: none; /* Non-prefixed version, currently
                                      supported by Chrome and Opera */
    }

    .slidecontainer {
      width: 100%;
    }

    .slider {
      -webkit-appearance: none;
      width: 90%;
      height: 15px;
      border-radius: 5px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }

    .slider:hover {
      opacity: 1;
    }
  
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 30px;
      height: 30px;
      border-radius: 50%;
      background: green;
      cursor: pointer;
    }

    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: green;
      cursor: pointer;
    }

      
    </style>
</head>
<body class="noselect">
    <table id="mainTable" style="width:400px;margin:auto;table-layout:fixed" CELLSPACING=10>
      <tr>
        <td></td>
        <td class="button" ontouchstart='start("F")' ontouchend='stop()'><span class="arrows" >&#8679;</span></td>
        <td></td>
      </tr>
      <tr>
        <td class="button" ontouchstart='start("R")' ontouchend='stop()'><span class="arrows" >&#8678;</span></td>
        <td class="button"><p id="info"></p></td>    
        <td class="button" ontouchstart='start("L")' ontouchend='stop()'><span class="arrows" >&#8680;</span></td>
      </tr>
      <tr>
        <td></td>
        <td class="button" ontouchstart='start("B")' ontouchend='stop()'><span class="arrows" >&#8681;</span></td>
        <td></td>
      </tr>
      <tr/><tr/>
      <tr>
        <td style="text-align:left"><b>Speed:</b></td>
        <td colspan=2>
         <div class="slidecontainer">
            <input type="range" min="0" max="10" value="10" class="slider" id="Speed" oninput='start(value)'>
          </div>
        </td>
      </tr>
      <tr>
        <td style="text-align:left"><b>Mode:</b></td>
        <td colspan=2>
          <div>
            <label>
              <input type="radio" name="mode" value="auto" onchange="auto()">Auto
            </label>
            <label>
              <input type="radio" name="mode" value="manual" checked onchange="stop()">Manual
            </label>
          </div>
        </td>
      </tr>
    </table>
    <script>
    const info = document.getElementById('info')
info.innerText = 'stop'
let manual = true

function start(text) {
  if(!manual){
    alert('matikan mode auto!!')
    return
  }
  var xhttp = new XMLHttpRequest()
  xhttp.open("GET", `/control?State=${text}` ,true)
  xhttp.send()
  info.innerText = text
}

function stop() {
  var xhttp = new XMLHttpRequest()
  xhttp.open("GET", "/control?State=S",true)
  xhttp.send()
  manual = true
  info.innerText = "stop"
}

function auto(){
  var xhttp = new XMLHttpRequest()
  xhttp.open("GET", "/control?State=A",true)
  xhttp.send()
  manual = false
  info.innerText = 'auto'
}
</script>
</body>
</html>
  )=====";