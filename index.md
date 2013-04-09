---
layout: default
title: flohofwoe.net
---

## Welcome Earthling

You probably came here to have a look at the <a href="demos.html">Nebula3/emscripten Demos</a>.
If you're interested in the technical details I recommend you spend a bit of time on my
<a href="http://flohofwoe.blogspot.com" target="_blank">tech blog</a>.

## What's New

{% for post in site.tags.news %}
---
<a href="{{ post.url }}">
<h3>{{ post.title }}</h3> 
<span class="date">
<div class="dateday">{{ post.date | date: "%e" }}</div>
<div>{{ post.date | date: "%b" }}</div>
<div class="dateyear">{{ post.date | date: "%Y" }}</div>
</span>
</a>

{{ post.content }}
{% endfor %}
