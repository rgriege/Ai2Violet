/*jslint vars: true, plusplus: true, devel: true, nomen: true, regexp: true, indent: 4, maxerr: 50 */
/*global $, window, location, CSInterface, SystemPath, themeManager*/

// TODO(rgriege): replace me
// var hooks = JSON.parse('{"hooks":{"Text":{"get":{"unit":["s","a","b","c"],"type":["area","cnt","park","pcnt"]},"nrsf":{},"ttl_park":{}},"Button":{"add":{"unit":["s","a","b","c"],"type":["area","cnt","park","pcnt"]},"sub":{"unit":["s","a","b","c"],"type":["area","cnt","park","pcnt"]},"reset":{},"set":{}}}}').hooks;
var hooks = {};

function handleArtSelection(event) {
	'use strict';
	console.log('handleArtSelection');
	var i;
	var hookForm = document.getElementById('hookForm');
	
	if (Object.getOwnPropertyNames(hooks).length > 0 && hooks[event.data.artType]) {
		// clear existing select
		var hookSelect = document.getElementById('hookSelect');
		while (hookSelect.firstChild) {
			hookSelect.removeChild(hookSelect.firstChild);
		}
		
		// add appropriate options
		var emptyOption = document.createElement('option');
		emptyOption.textContent = i;
		emptyOption.setAttribute('value', i);
		hookSelect.appendChild(emptyOption);
		for (i in hooks[event.data.artType]) {
			if (hooks[event.data.artType].hasOwnProperty(i)) {
				var option = document.createElement('option');
				option.textContent = i;
				option.setAttribute('value', i);
				hookSelect.appendChild(option);
			}
		}
		
		hookSelect.value = event.data.hook;
		// TODO(rgriege): param selection
		/*if (hookSelect.value) {
		
		}*/
		
		// ensure not hidden
		hookForm.hidden = false;
	} else {
		// ensure hidden
		hookForm.hidden = true;
	}
	
	document.getElementById('header').textContent = event.data.artType;
}

function handleHookFile(event) {
	'use strict';
	console.log('handleHookFile');
	document.getElementById('hookFileBtn').textContent = event.data.hookFileName;
	hooks = event.data.hooks;
}

function hookSelectChanged(event) {
	'use strict';
	var artType = document.getElementById('header').textContent;
	// TODO(rgriege): param selection
	/*var paramList = hooks[artType][event.target.value];
	if (paramList && Object.getOwnPropertyNames(paramList).length > 0) {
		
	}*/
	
	var csEvent = new CSEvent('com.adobe.csxs.events.Ai2Vlt.SelectHook', 'APPLICATION', 'ILST', 'com.bf.Ai2VltUi');
	csEvent.data = event.target.value;
	var csInterface = new CSInterface();
	csInterface.dispatchEvent(csEvent);
}

function chooseHookFile(event) {
	'use strict';
	event.preventDefault();
	var csEvent = new CSEvent('com.adobe.csxs.events.Ai2Vlt.LoadHooks', 'APPLICATION', 'ILST', 'com.bf.Ai2VltUi');
	var csInterface = new CSInterface();
	csInterface.dispatchEvent(csEvent);
}


(function () {
	'use strict';
    
	var csInterface = new CSInterface();
	csInterface.addEventListener('com.adobe.csxs.events.Ai2Vlt.UpdateSelection', handleArtSelection);
	csInterface.addEventListener('com.adobe.csxs.events.Ai2Vlt.UpdateHookFile', handleHookFile);
        
	themeManager.init();
	
	window.onload = function () {
		document.getElementById('hookFileBtn').onclick = chooseHookFile;		
		document.getElementById('hookSelect').onchange = hookSelectChanged;
		
		// TODO(rgriege): remove me
		/*var event = {};
		event.data = JSON.parse('{"artType":"Text"}');
		handleArtSelection(event);*/
	};
}());