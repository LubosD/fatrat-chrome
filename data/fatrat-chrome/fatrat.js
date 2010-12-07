function linkOnClick(info, tab) {
	plugin = document.getElementById('fatrat-npapi');
	plugin.handleLink(info.linkUrl);
}

function selectionOnClick(info, tab) {
	plugin = document.getElementById('fatrat-npapi');
	plugin.handleLink(info.selectionText);
}

chrome.contextMenus.create({"title": 'Download Link with FatRat', "contexts":['link'], "onclick": linkOnClick});
chrome.contextMenus.create({"title": 'Download with FatRat', "contexts":['selection'], "onclick": selectionOnClick});

