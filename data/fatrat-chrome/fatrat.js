function linkOnClick(info, tab) {
	plugin = document.getElementById('fatrat-npapi');
	plugin.handleLink(info.linkUrl);
}

function selectionOnClick(info, tab) {
	plugin = document.getElementById('fatrat-npapi');
	plugin.handleLink(info.selectionText);
}

fatratTabs = [];

chrome.contextMenus.create({"title": 'Download Link with FatRat', "contexts":['link'], "onclick": linkOnClick});
chrome.contextMenus.create({"title": 'Download with FatRat', "contexts":['selection'], "onclick": selectionOnClick});

chrome.extension.onRequest.addListener(
	function(request, sender, sendResponse) {
		if (request.status == "fatrat") {
			console.log(request.status);
			if (!fatratTabs[sender.tab.id]) {
				var hn = getHostname(sender.tab.url);
				var mid = chrome.contextMenus.create({"title": 'Download Link with FatRat at '+hn, "contexts":['link'], "onclick": function(info) { linkOnClickRemote(info, sender.tab.id); } });
				var mid2 = chrome.contextMenus.create({"title": 'Download with FatRat at '+hn, "contexts":['selection'], "onclick": function(info) { selectionOnClickRemote(info, sender.tab.id); } });
				fatratTabs[sender.tab.id] = { hostname: hn, menuIdLink : mid, menuIdSel : mid2 };
			}
		} else if (request.status == "other" && fatratTabs[sender.tab.id]) {
			chrome.contextMenus.remove(fatratTabs[sender.tab.id].menuIdLink);
			chrome.contextMenus.remove(fatratTabs[sender.tab.id].menuIdSel);
			delete fatratTabs[sender.tab.id];
		}
	}
);

chrome.tabs.onRemoved.addListener(function(tabId) {
	if (fatratTabs[sender.tab.id]) {
		chrome.contextMenus.remove(fatratTabs[sender.tab.id].menuIdLink);
		chrome.contextMenus.remove(fatratTabs[sender.tab.id].menuIdSel);
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
	link = info.linkUrl;
	remoteStart(tabId, link);
}

function selectionOnClickRemote(info, tabId) {
	console.log("Link submit to tabId "+tabId);
	link = info.selectionText;
	remoteStart(tabId, link);
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
