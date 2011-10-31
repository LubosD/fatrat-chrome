function linkOnClick(info, tab) {
	plugin = document.getElementById('fatrat-npapi');
	var url;
	
	if (info.linkUrl)
		url = info.linkUrl;
	else if (info.selectionText)
		url = info.selectionText;
	else if (info.srcUrl)
		url = info.srcUrl;
	else if (info.frameUrl)
		url = info.frameUrl;
	else if (info.pageUrl)
		url = info.pageUrl;
	plugin.handleLink(url);
}

fatratTabs = [];

chrome.contextMenus.create({"title": 'Download with FatRat', "contexts":['all'], "onclick": linkOnClick});

chrome.extension.onRequest.addListener(
	function(request, sender, sendResponse) {
		if (request.status == "fatrat") {
			console.log(request.status);
			if (!fatratTabs[sender.tab.id]) {
				var hn = getHostname(sender.tab.url);
				var mid = chrome.contextMenus.create({"title": 'Download with FatRat at '+hn, "contexts":['all'], "onclick": function(info) { linkOnClickRemote(info, sender.tab.id); } });
				fatratTabs[sender.tab.id] = { hostname: hn, menuIdLink : mid };
			}
		} else if (request.status == "other" && fatratTabs[sender.tab.id]) {
			chrome.contextMenus.remove(fatratTabs[sender.tab.id].menuIdLink);
			delete fatratTabs[sender.tab.id];
		}
	}
);

chrome.tabs.onRemoved.addListener(function(tabId) {
	if (fatratTabs[tabId]) {
		chrome.contextMenus.remove(fatratTabs[tabId].menuIdLink);
		chrome.contextMenus.remove(fatratTabs[tabId].menuIdSel);
		delete fatratTabs[sender.tab.id];
	}
});

chrome.tabs.onUpdated.addListener(function(tabId, changeInfo, tab) {
	chrome.tabs.executeScript(tabId, {file: "content_script.js"});
});

chrome.windows.getAll({ populate: true }, function(wins) {
	for (var i=0;i<wins.length;i++) {
		var win = wins[i];
		for (var j=0;j<win.tabs.length;j++) {
			var tab = win.tabs[j];
			chrome.tabs.executeScript(tab.id, {file: "content_script.js"});
		}
	}
});

function getHostname(str) {
	var re = new RegExp('^(?:f|ht)tp(?:s)?\://([^/]+)', 'im');
	return str.match(re)[1].toString();
}

function linkOnClickRemote(info, tabId) {
	console.log("Link submit to tabId "+tabId);
	
	var url;
	
	if (info.linkUrl)
		url = info.linkUrl;
	else if (info.srcUrl)
		url = info.srcUrl;
	else if (info.frameUrl)
		url = info.frameUrl;
	else if (info.pageUrl)
		url = info.pageUrl;
	else if (info.selectionText)
		url = info.selectionText;
	
	remoteStart(tabId, url);
}

function remoteStart(tabId, link) {
	script = "var customEvent = document.createEvent('Event');"+
		"customEvent.initEvent('startDownload', true, true);"+
		"el = document.getElementById('fatrat-chrome-comm-div');"+
		"el.innerHTML = '"+link+"';"+
		"el.dispatchEvent(customEvent);";
	
	chrome.tabs.executeScript(tabId, {code: script});
	chrome.tabs.update(tabId, {selected: true});
}
