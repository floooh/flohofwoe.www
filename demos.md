---
layout: default
title: flohofwoe.net
---

## Nebula3/emscripten Demos

These are Nebula3 engine demos cross-compiled to Javascript+WebGL using 
<a href="https://github.com/kripken/emscripten" target="_blank">emscripten</a>. Please use
an uptodate WebGL-enabled browser (Chrome or Firefox)

All assets used by courtesy of <a href="http://www.drakensang.com" target="_blank">Drakensang Online</a>,
the game I'm working on during day time.

## Dragons

Renders a lot of dragons (add more dragons with the <b>cursor up</b> key). Tests Javascript and WebGL 
draw call performance. Animations and joint skeletons are evaluated in Javascript indidually for each dragon.
One dragon needs 2 uniform updates and 1 draw-call per geometry pass, the character model has
54 joints and 3.1k triangles. At 961 dragons, this means: 52k joints animated and evaluated,
~6 million triangles rendered, and 1922 draw calls.

<div class="image">
    <p><a href="demos/dragons.html"><img src="images/dragons.jpg" width="640"/></a></p>
    <p><a href="demos/dragons.html">regular</a> | <a href="demos/dragons_asmjs.html">asm.js</a></p>
</div>
cursor up: more dragons<br>
cursor down: less dragons<br>
space: reset camera<br>

## Drakensang Online Map Viewer

This is the map rendering code from the Drakensang Online client with correct realtime 
lighting and a few animated characters thrown in, plus collision test. Move the player 
ghoul around by clicking or holding the left mouse button. The demo tests mainly
WebGL performance, there are about 3k WebGL calls per frame.

<div class="image">
    <p><a href="demos/dsomapviewer.html"><img src="images/dsomapviewer.jpg" width="640"/></a></p>
    <p><a href="demos/dsomapviewer.html">regular</a> | <a href="demos/dsomapviewer_asmjs.html">asm.js</a></p>
</div>
Move around with left-mouse-button.

## Drakensang Online Character Model Viewer

Display a selection of Drakensang Online character models and animations.

<div class="image">
    <p><a href="demos/dsocharviewer.html"><img src="images/dsocharviewer.jpg" width="640"/></a></p>
    <p><a href="demos/dsocharviewer.html">regular</a> | <a href="demos/dsocharviewer_asmjs.html">asm.js</a></p>
</div>
cursor up: next character<br>
cursor down: previous character<br>
cursor left: next animation<br>
cursor right: next skin<br>

## Instancing Test

This tests the fake instance-rendering I implemented for platforms without hardware instancing
support. This uses "unrolled" vertex- and index-buffers, and a vertex shader matrix palette with
instance transforms. Currently renders up to 64 instances in a single draw-call (limited by
number of vertex uniforms, which is 254 in ANGLE).

<div class="image">
    <p><a href="demos/instancing.html"><img src="images/instancing.jpg" width="640"/></a></p>
    <p><a href="demos/instancing.html">regular</a> | <a href="demos/instancing_asmjs.html">asm.js</a></p>
</div>
cursor up: more cubes<br>
cursor down: less cubes<br>
space: DISCO!<br>


