//------------------------------------------------------------------------------
//	messloader.js
//------------------------------------------------------------------------------

// describe ROM and Game Images here
var gamename = 'CAVE.KCC';
var game_file = 'CAVE.KCC';
var bios_filenames = 'kc85_3.zip'.split(' ');
var bios_files = {};
var file_countdown = 0;
if (bios_filenames.length !== 0 && bios_filenames[0] !== '') {
  file_countdown += bios_filenames.length;
}
if (gamename !== '' && gamename !== 'GAME_FILE') {
  file_countdown++;
}

// load a file through XHR
var fetch_file = function(url, cb) {
	var xhr = new XMLHttpRequest();
	xhr.open("GET", url, true);
	xhr.responseType = "arraybuffer";
	xhr.onload = function(e) {
		var ints = new Int8Array(xhr.response);
		cb(ints);
	};
	xhr.send();
};

// emscripten integration
var Module = {
	'arguments': ["kc85_3","-verbose","-rompath",".","-quik",game_file,"-window","-resolution","640x512","-nokeepaspect"],
    preRun: [],
    postRun: [],	
    print: (function() {
            var element = document.getElementById('output');
            element.value = ''; // clear browser cache
            return function(text) {
                text = Array.prototype.slice.call(arguments).join(' ');
                // These replacements are necessary if you render to raw HTML
                //text = text.replace(/&/g, "&amp;");
                //text = text.replace(/</g, "&lt;");
                //text = text.replace(/>/g, "&gt;");
                //text = text.replace('\n', '<br>', 'g');
                element.value += text + "\n";
                element.scrollTop = 99999; // focus on bottom
            };
    })(),
    printErr: function(text) {
        text = Array.prototype.slice.call(arguments).join(' ');
        if (0) { // XXX disabled for safety typeof dump == 'function') {
            dump(text + '\n'); // fast, straight to the real console
        } else {
            console.log(text);
        }
    },    
	canvas: document.getElementById('canvas'),
	noInitialRun: false,
	preInit: function() {
		// Load the downloaded binary files into the filesystem.
		for (var bios_fname in bios_files) {
			if (bios_files.hasOwnProperty(bios_fname)) {
				Module['FS_createDataFile']('/', bios_fname, bios_files[bios_fname], true, true);
			}
		}
		if (gamename !== "" && gamename !== "GAME_FILE") {
			Module['FS_createDataFile']('/', gamename, game_file, true, true);
		}
	}
};

// load and start emu script after all required files have been loaded
var update_countdown = function() {
  file_countdown -= 1;
  if (file_countdown === 0) {
    var headID = document.getElementsByTagName("body")[0];
		var newScript = document.createElement('script');
		newScript.type = 'text/javascript';
		newScript.src = 'messkc85_3.js';
		headID.appendChild(newScript);
  }
};

// Fetch the BIOS and the game we want to run.
for (var i=0; i < bios_filenames.length; i++) {
  var fname = bios_filenames[i];
  if (fname === "") {
    continue;
  }
  fetch_file(fname, function(data) { bios_files[fname] = data; update_countdown(); });
}

if (gamename !== "" && gamename !== "GAME_FILE") {
  fetch_file(gamename, function(data) { game_file = data; update_countdown(); });
}
