/*jslint vars: true, plusplus: true, devel: true, nomen: true, regexp: true, indent: 4, maxerr: 50 */
/*global $, window, location, CSInterface, SystemPath, themeManager*/


function handleSelection(event) {
	'use strict';
	console.log(event.data);
	$('#header').text(event.data);
}


(function () {
	'use strict';

	var csInterface = new CSInterface();
    
	csInterface.addEventListener('com.adobe.csxs.events.Ai2Vlt.UpdateSelection', handleSelection);
        
	themeManager.init();
}());