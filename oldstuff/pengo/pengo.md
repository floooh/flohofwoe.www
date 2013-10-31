---
layout: oldstuff
title: Pengo
---

My Pengo clone running in JSMESS. The game will start automatically after a few seconds. PS: I had to manually patch the auto-start address in the PENGO.KCC file to point to the right start address (which is 0x1D44). For some
reason the .KCC file had an autostart address which didn't actually work 
(and I don't remember putting one into the original game).

<div class="emscripten_border">
    <canvas class="emscripten" id="canvas" width="640" height="512" oncontextmenu="event.preventDefault()"></canvas>
</div>
<textarea class="emscripten" id="output" rows="3" cols="80" > </textarea>
<script type='text/javascript' src='messloader.js'> </script>

## TODO: more information about the game!
