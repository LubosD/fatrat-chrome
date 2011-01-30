
elem = document.getElementById('fatrat-chrome-comm-div');
if (elem)
	chrome.extension.sendRequest({status: "fatrat"});
else
	chrome.extension.sendRequest({status: "other"});
