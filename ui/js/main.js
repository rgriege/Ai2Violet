/*jslint vars: true, plusplus: true, devel: true, nomen: true, regexp: true, indent: 4, maxerr: 50 */
/*global $, window, location, CSInterface, SystemPath, themeManager*/

var paramLists = {};
var hooks = {};

function hookSelectChanged(event) {
	'use strict';
	console.log('hookSelectChanged ' + event.target.value);
	var artType = event.target.getAttribute('id').slice(0, -10);
	
	var paramSelectContainer = document.getElementById('paramSelectContainer');
	for (var i = 0; i < paramSelectContainer.children.length; ++i) {
		paramSelectContainer.children[i].hidden = true;
	}
	
	var params = hooks[artType][event.target.value];
	if (params) {
		for (var i = 0; i < params.length; ++i) {
			document.getElementById(params[i] + 'ParamForm').hidden = false;
			document.getElementById(params[i] + 'ParamSelect').value = '';
		}
	}
	
	var csEvent = new CSEvent('com.adobe.csxs.events.Ai2Vlt.SelectHook', 'APPLICATION', 'ILST', 'com.bf.Ai2VltUi');
	csEvent.data = event.target.value;
	var csInterface = new CSInterface();
	csInterface.dispatchEvent(csEvent);
}

function paramSelectChanged(event) {
	'use strict';
	var paramType = event.target.getAttribute('id').slice(0, -11);
	var param = event.target.value;
	console.log('paramSelectChanged ' + paramType + ':' + param);
	
	var csEvent = new CSEvent('com.adobe.csxs.events.Ai2Vlt.SelectParam', 'APPLICATION', 'ILST', 'com.bf.Ai2VltUi');
	csEvent.data = paramType + ':' + param;
	var csInterface = new CSInterface();
	csInterface.dispatchEvent(csEvent);
}

function handleArtSelection(event) {
	'use strict';
	console.log('handleArtSelection');
	
	document.getElementById('artType').textContent = event.data.artType;
	
	// hide and clear all hook & param forms
	var hookSelectContainer = document.getElementById('hookSelectContainer');
	for (var i = 0; i < hookSelectContainer.children.length; ++i) {
		hookSelectContainer.children[i].hidden = true;
	}
	for (var artType in hooks) {
		document.getElementById(artType + 'HookSelect').value = '';
	}
	var paramSelectContainer = document.getElementById('paramSelectContainer');
	for (var i = 0; i < paramSelectContainer.children.length; ++i) {
		paramSelectContainer.children[i].hidden = true;
	}
	for (var param in paramLists) {
		document.getElementById(param + 'ParamSelect').value = '';
	}
	
	if (hooks[event.data.artType]) {
		document.getElementById(event.data.artType + 'HookForm').hidden = false;
		document.getElementById(event.data.artType + 'HookSelect').value = event.data.hook;
		var params = hooks[event.data.artType][event.data.hook];
		if (params) {
			for (var i = 0; i < params.length; ++i) {
				var param = params[i];
				document.getElementById(param + 'ParamForm').hidden = false;
				if (event.data.params[param]) {
					document.getElementById(param + 'ParamSelect').value = event.data.params[param];
				}
			}
		}
	} else if (event.data.artType === 'Symbol') {
		var paramSelectContainer = document.getElementById('paramSelectContainer');
		for (var i = 0; i < paramSelectContainer.children.length; ++i) {
			paramSelectContainer.children[i].hidden = false;
		}
		for (var param in paramLists) {
			document.getElementById(param + 'ParamForm').hidden = false;
			if (event.data.params[param]) {
				document.getElementById(param + 'ParamSelect').value = event.data.params[param];
			}
		}
	}
}

function handleHookFile(event) {
	'use strict';
	console.log('handleHookFile');
	document.getElementById('hookFileBtn').textContent = event.data.hookFileName;
	
	hooks = event.data.hooks;
	var hookSelectContainer = document.getElementById('hookSelectContainer');
	while (hookSelectContainer.firstChild) {
		hookSelectContainer.removeChild(hookSelectContainer.firstChild);
	}
	var artType;
	for (artType in hooks) {
		var hookList = hooks[artType];
		var form = document.createElement('div');
		form.setAttribute('id', artType + 'HookForm');
		form.hidden = true;
		
		var label = document.createElement('label');
		label.textContent = 'Hook:';
		form.appendChild(label);
		
		var select = document.createElement('select');
		select.setAttribute('id', artType + 'HookSelect');
		
		var emptyOption = document.createElement('option');
		select.appendChild(emptyOption);
		
		for (var hook in hookList) {
			var option = document.createElement('option');
			option.textContent = hook;
			option.setAttribute('value', hook);
			select.appendChild(option);
		}
		select.onchange = hookSelectChanged;
		form.appendChild(select);
		
		hookSelectContainer.appendChild(form);
	}
	
	
	paramLists = event.data.paramLists;
	var paramSelectContainer = document.getElementById('paramSelectContainer');
	while (paramSelectContainer.firstChild) {
		paramSelectContainer.removeChild(paramSelectContainer.firstChild);
	}
	var param;
	for (param in paramLists) {
		var paramList = paramLists[param];
		var form = document.createElement('div');
		form.setAttribute('class', 'row');
		form.setAttribute('id', param + 'ParamForm');
		form.hidden = true;

		var label = document.createElement('label');
		label.textContent = param + ':';
		form.appendChild(label);

		var select = document.createElement('select');
		select.setAttribute('id', param + 'ParamSelect');
		
		var emptyOption = document.createElement('option');
		select.appendChild(emptyOption);

		for (var j in paramList) {
			var paramOpt = paramList[j];
			var option = document.createElement('option');
			option.textContent = paramOpt;
			option.setAttribute('value', paramOpt);
			select.appendChild(option);
		}
		select.onchange = paramSelectChanged;
		form.appendChild(select);

		paramSelectContainer.appendChild(form);
	}
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
		
		// TODO(rgriege): remove me
		/*var event = {};
		event.data = {};
		event.data.hookFileName = 'mixEngineHooks.json';
		event.data.paramLists = JSON.parse('{"unit":["s","a","b","c"],"type":["area","cnt","park","pcnt"]}');
		event.data.hooks = JSON.parse('{"Text":{"get":["unit","type"],"nrsf":{},"ttl_park":{}},"Button":{"add":["unit","type"],"sub":["unit","type"],"reset":{},"set":{}}}');
		handleHookFile(event);
		event.data = JSON.parse('{"artType":"Text","hook":"get","params":{"unit":"b"}}');
		handleArtSelection(event);*/
	};
}());