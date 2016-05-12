/*jslint vars: true, plusplus: true, devel: true, nomen: true, regexp: true, indent: 4, maxerr: 50 */
/*global $, window, location, CSInterface, SystemPath, themeManager*/


function handleSelection(event) {
	'use strict';
	$('#header').text(event.data);
}

function handleHookFile(event) {
	'use strict';
	$('#hookFileBtn').text(event.data);
}


(function () {
	'use strict';

	var csInterface = new CSInterface();
    
	csInterface.addEventListener('com.adobe.csxs.events.Ai2Vlt.UpdateSelection', handleSelection);
	csInterface.addEventListener('com.adobe.csxs.events.Ai2Vlt.UpdateHookFile', handleHookFile);
        
	themeManager.init();
	
	window.onload = function () {
		document.getElementById('hookFileBtn').onclick = function (e) {
			e.preventDefault();
			var event = new CSEvent('com.adobe.csxs.events.Ai2Vlt.LoadHooks', 'APPLICATION', 'ILST', 'com.bf.Ai2VltUi');
			csInterface.dispatchEvent(event);
		};
	};
}());