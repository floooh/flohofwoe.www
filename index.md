---
layout: default
title: flohofwoe.net
---

## Welcome

This is the personal home page of Andre 'Floh' Weissflog. You probably
came here for the <a href="demos.html" target="_blank">Nebula3/emscripten demos</a>.

If you're interested in the tech behind these demos, check out my 
<a href="http://flohofwoe.blogspot.com" target="_blank">tech-blog</a>.

If you want to contact me, find me on 
<a href="http://www.facebook.com/andre.weissflog.3" target="_blank">Facebook</a> or on LinkedIn.

This page has been created with <a href="http://jekyllrb.com/" target="_blank">Jekyll</a>
and most of the layout has been blatantly ripped from the 
<a href="http://www.glfw.org/" target="_blank">GLFW web site</a>. I'll work on making
it a less obvious copy in the near future :)

## What's New

Nothing yet.

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
