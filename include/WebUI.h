#ifndef WEBUI_H
#define WEBUI_H

const char* index_html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <title>D.A.V.I.D.E. Studio</title>
  <style>
    body {
      background: linear-gradient(135deg, #0a0a0a 0%, #1a1a1a 50%, #0a0a0a 100%);
      margin: 0; padding: 30px 10px; font-family: 'Arial Black', sans-serif;
      user-select: none; -webkit-user-select: none;
      display: flex; flex-direction: column; align-items: center; min-height: 100vh;
    }

    h1 { color: #fff; text-shadow: 0 0 20px #ffaa00, 0 0 40px rgba(255,170,0,0.5); text-align: center; margin-bottom: 5px; font-size: 2.5em; letter-spacing: 4px; }
    p.subtitle { color: #aaa; letter-spacing: 4px; text-align: center; margin-bottom: 40px; font-size: 0.9em; text-shadow: 0 2px 5px rgba(0,0,0,0.8); }

    /* Pedalboard */
    .pedalboard {
      display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; max-width: 1300px;
      background: linear-gradient(90deg, #2a2416 0%, #3a3428 25%, #2a2416 50%, #3a3428 75%, #2a2416 100%);
      padding: 30px; border-radius: 15px;
      box-shadow: 0 20px 40px rgba(0,0,0,0.9), inset 0 1px 1px rgba(255,255,255,0.1), inset 0 -1px 1px rgba(0,0,0,0.5); border: 3px solid #1a1410;
    }

    /* Pedal Chassis */
    .pedal {
      width: 220px; height: 380px; border-radius: 8px; border: 1px solid #000;
      box-shadow: 20px 20px 40px rgba(0,0,0,0.95), -1px -1px 3px rgba(255,255,255,0.05), inset 0 1px 0 rgba(255,255,255,0.2), inset 0 -2px 4px rgba(0,0,0,0.7), inset 0 0 0 1px rgba(0,0,0,0.3);
      display: flex; flex-direction: column; align-items: center; position: relative; overflow: visible;
      transform: perspective(1200px) rotateX(0.5deg) rotateY(0.5deg); transition: transform 0.3s ease;
    }
    .pedal::before { content: ''; position: absolute; top: 0; left: 0; right: 0; bottom: 0; background: linear-gradient(180deg, rgba(255,255,255,0.15) 0%, transparent 30%, rgba(0,0,0,0.2) 100%), repeating-linear-gradient(90deg, transparent, transparent 2px, rgba(0,0,0,0.02) 2px, rgba(0,0,0,0.02) 4px); pointer-events: none; border-radius: 8px; }
    .pedal::after { content: ''; position: absolute; bottom: 0; left: 0; right: 0; height: 8px; background: linear-gradient(180deg, transparent, rgba(0,0,0,0.3)); border-radius: 0 0 8px 8px; pointer-events: none; }
    .pedal-title { color: #fff; margin-top: 22px; font-size: 1.1em; letter-spacing: 1.5px; text-shadow: 1px 1px 2px rgba(0,0,0,0.95); text-align: center; font-weight: bold; text-transform: uppercase; position: relative; z-index: 2; }

    /* LEDs */
    .led { width: 18px; height: 18px; background: radial-gradient(circle at 25% 25%, #1a0000, #050000); border-radius: 50%; margin-top: 16px; border: 2px solid #0a0a0a; box-shadow: inset -2px -2px 4px rgba(0,0,0,0.9), inset 1px 1px 2px rgba(0,0,0,0.4), inset 0 0 3px rgba(0,0,0,0.6), 0 0 6px rgba(0,0,0,0.6); transition: all 0.15s; position: relative; z-index: 2; }
    .led::before { content: ''; position: absolute; top: 2px; left: 2px; width: 5px; height: 5px; background: radial-gradient(circle, rgba(100,100,100,0.4), transparent); border-radius: 50%; }

    /* LED Colors */
    .pedal.active .led { background: radial-gradient(circle at 25% 25%, #ff5555, #cc0000); box-shadow: 0 0 20px #ff3333, 0 0 40px rgba(255,0,0,0.7), inset -2px -2px 4px rgba(0,0,0,0.6), inset 1px 1px 2px rgba(255,180,180,0.3); }
    .pedal.active#pedal0 .led { background: radial-gradient(circle at 25% 25%, #55ff55, #00dd00); box-shadow: 0 0 20px #33ff33, 0 0 40px rgba(0,255,0,0.7); }
    .pedal.active#pedal1 .led { background: radial-gradient(circle at 25% 25%, #cc66ff, #9900cc); box-shadow: 0 0 20px #cc66ff, 0 0 40px rgba(153,0,204,0.7); }
    .pedal.active#pedal2 .led { background: radial-gradient(circle at 25% 25%, #ff5555, #cc0000); box-shadow: 0 0 20px #ff3333, 0 0 40px rgba(255,0,0,0.7); }
    .pedal.active#pedal3 .led { background: radial-gradient(circle at 25% 25%, #55ddff, #0099ff); box-shadow: 0 0 20px #33ccff, 0 0 40px rgba(0,170,255,0.7); }
    .pedal.active#pedal4 .led { background: radial-gradient(circle at 25% 25%, #ff55ff, #dd00ff); box-shadow: 0 0 20px #ff33ff, 0 0 40px rgba(220,0,255,0.7); }
    .pedal.active#pedal5 .led { background: radial-gradient(circle at 25% 25%, #55ffdd, #00ddbb); box-shadow: 0 0 20px #33ffcc, 0 0 40px rgba(0,220,200,0.7); }
    .pedal.active#pedal6 .led { background: radial-gradient(circle at 25% 25%, #00ffcc, #009999); box-shadow: 0 0 20px #00ffcc, 0 0 40px rgba(0,255,204,0.7); } /* Tuner LED */

    /* Footswitch */
    .footswitch { width: 52px; height: 52px; background: radial-gradient(circle at 30% 30%, #ffffff, #e8e8e8 25%, #c0c0c0 50%, #808080 75%, #404040); border-radius: 50%; border: 2px solid #1a1a1a; position: absolute; bottom: 25px; box-shadow: 6px 6px 14px rgba(0,0,0,0.85), inset -3px -3px 6px rgba(0,0,0,0.7), inset 2px 2px 5px rgba(255,255,255,0.5), inset 0 0 10px rgba(0,0,0,0.4), 0 2px 4px rgba(255,255,255,0.1); cursor: pointer; outline: none; transition: all 0.1s; z-index: 3; }
    .footswitch:active { transform: translateY(2px) scale(0.97); box-shadow: 2px 2px 6px rgba(0,0,0,0.8), inset -2px -2px 4px rgba(0,0,0,0.8); }

    /* Controls & Knobs */
    .controls { display: flex; width: 100%; justify-content: space-around; margin-top: 25px; padding: 0 12px; position: relative; z-index: 2; }
    .knob-group { display: flex; flex-direction: column; align-items: center; }
    .knob-label { color: #ddd; font-size: 0.68em; margin-top: 10px; letter-spacing: 1px; text-shadow: 1px 1px 2px rgba(0,0,0,0.95); font-weight: bold; text-transform: uppercase; }
    .knob-value { color: #ffdd66; font-family: 'Courier New', monospace; font-size: 0.78em; margin-top: 4px; background: radial-gradient(circle at 20% 20%, #2a2a2a, #0a0a0a); padding: 3px 8px; border-radius: 2px; box-shadow: inset 0 2px 4px rgba(0,0,0,0.9), inset 0 -1px 2px rgba(0,0,0,0.6); border: 1px solid #1a1a1a; }
    
    .knob-surround { width: 60px; height: 60px; background: radial-gradient(circle at 25% 25%, #555, #222 40%, #0a0a0a 100%); border-radius: 50%; box-shadow: 0 10px 18px rgba(0,0,0,0.95), inset 0 2px 6px rgba(255,255,255,0.15), inset 0 -2px 6px rgba(0,0,0,0.8); display: flex; justify-content: center; align-items: center; border: 2px solid #000; position: relative; }
    .knob-dial { width: 44px; height: 44px; background: radial-gradient(circle at 30% 30%, #444, #1a1a1a 35%, #000 100%); border-radius: 50%; box-shadow: inset -3px -3px 8px rgba(0,0,0,0.95), inset 2px 2px 5px rgba(255,255,255,0.15), 0 4px 8px rgba(0,0,0,0.6); position: relative; cursor: grab; transform: rotate(-135deg); touch-action: none; border: 1px solid #000; }
    .knob-dial:active { cursor: grabbing; filter: brightness(1.15); }
    .knob-indicator { position: absolute; width: 4px; height: 14px; background: linear-gradient(180deg, #ffee88, #ffaa00); border-radius: 2px; top: 2px; left: 50%; transform: translateX(-50%); box-shadow: 0 0 4px rgba(255,200,0,0.9), inset 0 1px 1px rgba(255,255,255,0.3); }

    /* Tuner Specific Styles */
    .tuner-screen {
      width: 80%; height: 90px; background: #050505; border: 3px inset #222; border-radius: 6px; margin-top: 30px;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      box-shadow: inset 0 0 15px rgba(0,0,0,0.9); z-index: 2; position: relative;
    }
    .pedal.active#pedal6 .tuner-screen { box-shadow: inset 0 0 20px rgba(0, 255, 204, 0.15); }
    #tunerNote { font-size: 3em; font-family: monospace; font-weight: bold; color: #333; transition: color 0.1s; line-height: 1; margin-top: 5px; }
    #tunerCents { font-size: 0.8em; font-family: monospace; color: #333; margin-top: 5px; transition: color 0.1s; letter-spacing: 1px;}
    
    .pedal.active#pedal6 #tunerNote { color: #00ffcc; text-shadow: 0 0 10px rgba(0,255,204,0.5); }
    .pedal.active#pedal6 #tunerCents { color: #00ffcc; }

    /* Tuner Arc and Needle */
    .tuner-meter { position: relative; width: 140px; height: 60px; margin-top: 5px; overflow: hidden; border-bottom: 2px solid #333; }
    .meter-bg { position: absolute; width: 140px; height: 140px; border-radius: 50%; border: 3px dashed #444; top: 10px; left: 0; box-sizing: border-box; }
    .tuning-needle { position: absolute; width: 4px; height: 65px; background: #ff5555; bottom: 0; left: calc(50% - 2px); transform-origin: bottom center; transition: transform 0.15s ease-out; border-radius: 2px 2px 0 0; z-index: 10; transform: rotate(0deg); }

    /* Looper Buttons inside standard pedal */
    .looper-controls { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; width: calc(100% - 40px); margin: 35px auto 0 auto; z-index: 3; position: relative; }
    .btn-loop { padding: 12px 8px; font-size: 0.75em; font-family: 'Arial Black', sans-serif; background: linear-gradient(180deg, #2a2a2a 0%, #1a1a1a 100%); color: #666; border: 1px solid #444; border-radius: 5px; cursor: pointer; box-shadow: 0 4px 8px rgba(0,0,0,0.7), inset 0 -1px 2px rgba(0,0,0,0.6); display: flex; flex-direction: column; align-items: center; gap: 3px; }
    .btn-loop:active { transform: translateY(2px); box-shadow: 0 2px 4px rgba(0,0,0,0.8); }
    .btn-loop:nth-child(3) { grid-column: 1 / -1; }
    .btn-rec.active { color: #ff4444; border-color: #ff3333; background: linear-gradient(180deg, #3a1a1a 0%, #1a0a0a 100%); box-shadow: 0 0 15px rgba(255,51,51,0.6); }
    .btn-play.active { color: #44ff44; border-color: #33ff33; background: linear-gradient(180deg, #1a3a1a 0%, #0a1a0a 100%); box-shadow: 0 0 15px rgba(51,255,51,0.6); }
  </style>
</head>
<body>

  <h1>D.A.V.I.D.E.</h1>
  <p class="subtitle">MODULAR STUDIO RACK</p>

  <div class="pedalboard">
    
    <div class="pedal" style="background: linear-gradient(135deg, #daa520, #8b6508);">
      <div class="pedal-title" style="margin-top: 40px;">MASTER<br>BOOST</div>
      <div class="controls" style="margin-top: 50px;">
        <div class="knob-group">
          <div class="knob-surround"><div class="knob-dial" id="knobG"><div class="knob-indicator"></div></div></div>
          <div class="knob-label">VOLUME</div><div class="knob-value" id="valG">50</div>
        </div>
      </div>
    </div>

    <div class="pedal" id="pedal6" style="background: linear-gradient(135deg, #2b2b2b, #111111);">
      <div class="led" id="led6"></div>
      <div class="pedal-title" style="color: #ccc;">TUNER / MUTE</div>
      
      <div class="tuner-screen" style="height: 150px;">
        <div id="tunerNote">--</div>
        <div class="tuner-meter">
          <div class="meter-bg"></div>
          <div class="tuning-needle" id="myNeedle"></div>
        </div>
        <div id="tunerCents">MUTE</div>
      </div>
      
      <button class="footswitch" onclick="toggleFx(6)"></button>
    </div>

    <div class="pedal" id="pedal1" style="background: linear-gradient(135deg, #4b0082, #2e0050);">
      <div class="led" id="led1"></div>
      <div class="pedal-title">3-BAND EQ</div>
      <div class="controls" style="margin-top: 15px;">
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob1P"><div class="knob-indicator"></div></div></div><div class="knob-label">BASS</div><div class="knob-value" id="val1P">50</div></div>
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob1Q"><div class="knob-indicator"></div></div></div><div class="knob-label">MID</div><div class="knob-value" id="val1Q">50</div></div>
      </div>
      <div class="controls" style="margin-top: 10px;">
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob1R"><div class="knob-indicator"></div></div></div><div class="knob-label">TREBLE</div><div class="knob-value" id="val1R">50</div></div>
      </div>
      <button class="footswitch" onclick="toggleFx(1)"></button>
    </div>

    <div class="pedal" id="pedal0" style="background: linear-gradient(135deg, #009933, #004d1a);">
      <div class="led" id="led0"></div>
      <div class="pedal-title">TUBE DRIVE</div>
      <div class="controls">
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob0P"><div class="knob-indicator"></div></div></div><div class="knob-label">DRIVE</div><div class="knob-value" id="val0P">50</div></div>
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob0Q"><div class="knob-indicator"></div></div></div><div class="knob-label">LEVEL</div><div class="knob-value" id="val0Q">50</div></div>
      </div>
      <button class="footswitch" onclick="toggleFx(0)"></button>
    </div>

    <div class="pedal" id="pedal2" style="background: linear-gradient(135deg, #cc0000, #880000);">
      <div class="led" id="led2"></div>
      <div class="pedal-title">FUZZ FACE</div>
      <div class="controls">
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob2P"><div class="knob-indicator"></div></div></div><div class="knob-label">FUZZ</div><div class="knob-value" id="val2P">50</div></div>
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob2Q"><div class="knob-indicator"></div></div></div><div class="knob-label">LEVEL</div><div class="knob-value" id="val2Q">50</div></div>
      </div>
      <button class="footswitch" onclick="toggleFx(2)"></button>
    </div>

    <div class="pedal" id="pedal3" style="background: linear-gradient(135deg, #0088cc, #004466);">
      <div class="led" id="led3"></div>
      <div class="pedal-title">TREMOLO</div>
      <div class="controls">
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob3P"><div class="knob-indicator"></div></div></div><div class="knob-label">SPEED</div><div class="knob-value" id="val3P">50</div></div>
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob3Q"><div class="knob-indicator"></div></div></div><div class="knob-label">DEPTH</div><div class="knob-value" id="val3Q">50</div></div>
      </div>
      <button class="footswitch" onclick="toggleFx(3)"></button>
    </div>

    <div class="pedal" id="pedal4" style="background: linear-gradient(135deg, #6600cc, #330066);">
      <div class="led" id="led4"></div>
      <div class="pedal-title">TAPE DELAY</div>
      <div class="controls">
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob4P"><div class="knob-indicator"></div></div></div><div class="knob-label">TIME</div><div class="knob-value" id="val4P">50</div></div>
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob4Q"><div class="knob-indicator"></div></div></div><div class="knob-label">REPEATS</div><div class="knob-value" id="val4Q">50</div></div>
      </div>
      <button class="footswitch" onclick="toggleFx(4)"></button>
    </div>

    <div class="pedal" id="pedal5" style="background: linear-gradient(135deg, #00cca3, #006652);">
      <div class="led" id="led5"></div>
      <div class="pedal-title">REVERB</div>
      <div class="controls">
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob5P"><div class="knob-indicator"></div></div></div><div class="knob-label">SIZE</div><div class="knob-value" id="val5P">50</div></div>
        <div class="knob-group"><div class="knob-surround"><div class="knob-dial" id="knob5Q"><div class="knob-indicator"></div></div></div><div class="knob-label">MIX</div><div class="knob-value" id="val5Q">50</div></div>
      </div>
      <button class="footswitch" onclick="toggleFx(5)"></button>
    </div>

    <div class="pedal" style="background: linear-gradient(135deg, #1a3a2a, #0a1a1a);">
      <div class="led" id="ledLooper"></div>
      <div class="pedal-title">TAPE LOOPER</div>
      
      <div class="controls" style="margin-top: 15px;">
        <div class="knob-group">
          <div class="knob-surround"><div class="knob-dial" id="knobLprV"><div class="knob-indicator"></div></div></div>
          <div class="knob-label">VOLUME</div><div class="knob-value" id="valLprV">50</div>
        </div>
        <div class="knob-group">
          <div class="knob-surround"><div class="knob-dial" id="knobLprS"><div class="knob-indicator"></div></div></div>
          <div class="knob-label">SPEED</div><div class="knob-value" id="valLprS">50</div>
        </div>
      </div>

      <div class="looper-controls">
        <button id="btnL1" class="btn-loop btn-rec" onclick="setLooper(1)"><span class="btn-loop-icon">🔴</span><span class="btn-loop-text">Rec</span></button>
        <button id="btnL2" class="btn-loop btn-play" onclick="setLooper(2)"><span class="btn-loop-icon">▶️</span><span class="btn-loop-text">Play</span></button>
        <button id="btnL0" class="btn-loop" onclick="setLooper(0)"><span class="btn-loop-icon">⏹️</span><span class="btn-loop-text">Stop</span></button>
      </div>
    </div>

  </div>

  <script>
    var gateway = `ws://${window.location.hostname}:81/`;
    var websocket;
    
    let fxState = [false, false, false, false, false, false, false]; 

    const notes = [
        {name: "E2", freq: 82.41}, {name: "A2", freq: 110.00},
        {name: "D3", freq: 146.83}, {name: "G3", freq: 196.00},
        {name: "B3", freq: 246.94}, {name: "E4", freq: 329.63}
    ];

    window.addEventListener('load', () => { 
        websocket = new WebSocket(gateway); 
        
        websocket.onmessage = function(event) {
            let msg = event.data;
            if (msg.charAt(0) === 'T' && fxState[6]) { 
                let currentFreq = parseFloat(msg.substring(1));
                let noteEl = document.getElementById('tunerNote');
                let centsEl = document.getElementById('tunerCents');
                
                if (currentFreq < 10.0) {
                    noteEl.innerText = "--";
                    centsEl.innerText = "LISTENING";
                    noteEl.style.color = '#444';
                    centsEl.style.color = '#444';
                    noteEl.style.textShadow = 'none';
                    document.getElementById('myNeedle').style.transform = `rotate(0deg)`; 
                    return; 
                }
                
                let closestNote = notes[0];
                let minDiff = Math.abs(currentFreq - notes[0].freq);
                for(let i=1; i<notes.length; i++) {
                    let diff = Math.abs(currentFreq - notes[i].freq);
                    if(diff < minDiff) { minDiff = diff; closestNote = notes[i]; }
                }

                let cents = 1200 * Math.log2(currentFreq / closestNote.freq);
                let displayCents = Math.max(-50, Math.min(50, cents)); 
                let angle = (displayCents / 50) * 45; 
                document.getElementById('myNeedle').style.transform = `rotate(${angle}deg)`;

                noteEl.innerText = closestNote.name;
                
                if (Math.abs(cents) < 4) {
                    noteEl.style.color = '#33ff33';
                    noteEl.style.textShadow = '0 0 10px rgba(51,255,51,0.5)';
                    centsEl.style.color = '#33ff33';
                    centsEl.innerText = "IN TUNE";
                } else {
                    noteEl.style.color = '#00ffcc';
                    noteEl.style.textShadow = '0 0 10px rgba(0,255,204,0.5)';
                    centsEl.style.color = '#ffaa00';
                    
                    if (cents > 50) centsEl.innerText = "> +50 SHARP";
                    else if (cents < -50) centsEl.innerText = "< -50 FLAT";
                    else centsEl.innerText = displayCents > 0 ? "+" + Math.round(displayCents) + " SHARP" : Math.round(displayCents) + " FLAT";
                }
            }
        };
    });

    function sendData(cmd, val) { if (websocket && websocket.readyState === WebSocket.OPEN) websocket.send(cmd + val); }
    function sendParam(cmd, effId, val) { if (websocket && websocket.readyState === WebSocket.OPEN) websocket.send(cmd + effId + val); }

    function toggleFx(id) {
      fxState[id] = !fxState[id]; 
      const pedal = document.getElementById('pedal' + id);
      if(fxState[id]) { 
        pedal.classList.add('active'); 
        sendData("S", id + "1"); 
        if(id === 6) document.getElementById('tunerCents').innerText = "LISTENING";
      } else { 
        pedal.classList.remove('active'); 
        sendData("S", id + "0"); 
        if(id === 6) {
           document.getElementById('tunerNote').innerText = "--";
           document.getElementById('tunerCents').innerText = "MUTE";
           document.getElementById('tunerNote').style.color = '#333';
           document.getElementById('tunerCents').style.color = '#333';
           document.getElementById('tunerNote').style.textShadow = 'none';
        }
      }
    }

    function setLooper(state) {
      document.getElementById('btnL1').classList.remove('active');
      document.getElementById('btnL2').classList.remove('active');
      let led = document.getElementById('ledLooper');
      
      // Reset Looper LED
      led.style.background = 'radial-gradient(circle at 25% 25%, #1a0000, #050000)';
      led.style.boxShadow = 'inset -2px -2px 4px rgba(0,0,0,0.9), inset 1px 1px 2px rgba(0,0,0,0.4), inset 0 0 3px rgba(0,0,0,0.6), 0 0 6px rgba(0,0,0,0.6)';

      if(state == 1) { 
          document.getElementById('btnL1').classList.add('active'); 
          // Turn LED Red
          led.style.background = 'radial-gradient(circle at 25% 25%, #ff5555, #cc0000)';
          led.style.boxShadow = '0 0 20px #ff3333, 0 0 40px rgba(255,0,0,0.7), inset -2px -2px 4px rgba(0,0,0,0.6), inset 1px 1px 2px rgba(255,180,180,0.3)';
      }
      else if(state == 2) { 
          document.getElementById('btnL2').classList.add('active'); 
          // Turn LED Green
          led.style.background = 'radial-gradient(circle at 25% 25%, #55ff55, #00dd00)';
          led.style.boxShadow = '0 0 20px #33ff33, 0 0 40px rgba(0,255,0,0.7), inset -2px -2px 4px rgba(0,0,0,0.6), inset 1px 1px 2px rgba(180,255,180,0.3)';
      }

      sendData('L', state);
    }

    class RotaryKnob {
      constructor(knobId, displayId, initialValue, callback) {
        this.knob = document.getElementById(knobId); this.display = document.getElementById(displayId);
        this.value = initialValue; this.isDragging = false; this.startY = 0; this.startValue = 0;
        this.callback = callback; this.throttle = null;
        this.updateVisuals();
        this.knob.addEventListener('mousedown', (e) => this.startDrag(e.clientY));
        window.addEventListener('mousemove', (e) => this.drag(e.clientY));
        window.addEventListener('mouseup', () => this.stopDrag());
        this.knob.addEventListener('touchstart', (e) => { e.preventDefault(); this.startDrag(e.touches[0].clientY); }, {passive: false});
        window.addEventListener('touchmove', (e) => { if(this.isDragging){ e.preventDefault(); this.drag(e.touches[0].clientY); } }, {passive: false});
        window.addEventListener('touchend', () => this.stopDrag());
      }
      startDrag(y) { this.isDragging = true; this.startY = y; this.startValue = this.value; }
      stopDrag() { this.isDragging = false; }
      drag(y) {
        if (!this.isDragging) return;
        let deltaY = this.startY - y; 
        this.value = Math.max(0, Math.min(100, this.startValue + (deltaY / 1.5)));
        this.updateVisuals();
        clearTimeout(this.throttle);
        this.throttle = setTimeout(() => { if(this.callback) this.callback(Math.round(this.value)); }, 25);
      }
      updateVisuals() {
        let angle = (this.value / 100) * 270 - 135;
        this.knob.style.transform = `rotate(${angle}deg)`;
        this.display.innerText = Math.round(this.value);
      }
    }

    // Initialize all existing Knobs
    new RotaryKnob('knobG', 'valG', 50, (val) => sendData('G', val));
    for(let i = 0; i <= 5; i++) {
      new RotaryKnob(`knob${i}P`, `val${i}P`, 50, (val) => sendParam('P', i, val));
      new RotaryKnob(`knob${i}Q`, `val${i}Q`, 50, (val) => sendParam('Q', i, val));
    }
    new RotaryKnob('knob1R', 'val1R', 50, (val) => sendParam('R', 1, val));
    
    // Initialize the New Looper Knobs
    new RotaryKnob('knobLprV', 'valLprV', 50, (val) => sendData('V', val));
    new RotaryKnob('knobLprS', 'valLprS', 50, (val) => sendData('W', val));
  </script>
</body>
</html>
)=====";

#endif