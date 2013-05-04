---
layout: default
title: flohofwoe.net
---

## Source Code

Here's some highlevel source code which is useful to understand what's going on
in the demos.

Every demo is its own "executable" embedded as minified Javascript code into
a "shell" HTML page, this is what comes out of the emscripten linker stage
(you can also emit a pure JS script of course).

I've split the demos into 3 parts: 

1. A main.cc file, this simply instantiates an application object, and starts
the main loop.
2. A generical application class called EMSCTestApplication which derived from N3's
new PhasedApplication class (also shown). EMSCTestApplication simply holds
all the code which is identical for all demos, and some useful utility
methods.
3. An application class for each demo which is derived from EMSCTestApplication, 
these contain the "special parts".

### The main.cc file

Here's the main file of the Dragons demo. This basically contains the main() 
function, this looks a bit different from a typical hello-world: instead of
a main() function the entry point of a Nebula3 app is the NebulaMain() function,
and an ImplementNebulaApplication() macro. 

The reason for this is that not all target platforms have a standard main() function 
(e.g. Windows apps have WinMain()), or NebulaMain might not correspond at all to main(). For instance
on OSX, Android and NaCl, NebulaMain is actually executed in a thread which has been spawned by the
application main thread.

{% highlight cpp %}
{% include dragons.cc %}
{% endhighlight %}

### emsctestapplication.cc

This is the generic super-class of the Nebula3/emscripten demos. The EMSCTestApplication
class is derived from Nebula3's PhasedApplication base class, which implements
a "phased application model". An application goes through the phases Initial,
SetupPreloadQueue, Preloading, Opening, Running, Closing and Quit. These
phases decide themselves when they are ticked forward to the next phase.

{% highlight cpp %}
{% include emsctestapplication.cc %}
{% endhighlight %}

### phasedapplication.cc

This is Nebula3's PhasedApplication class for single-threaded platforms which
cannot "own the game loop". The basic idea is, that the application is called
back by some outer runtime, and must return within a small amount of time
(usually 16ms for 60fps apps, or 32ms for 30fps apps).

{% highlight cpp %}
{% include phasedapplication.cc %}
{% endhighlight %}

### dragonsapplication.cc

This is the application class of the Dragons demo, all it does is create
light sources, character models, handle input and override the
ResetCamera() method with a special version which tries to keep all
dragons in view after dragons are added or removed.

{% highlight cpp %}
{% include dragonsapplication.cc %}
{% endhighlight %}


