function MapWidget(latitude, longitude,  widgetName){
	this.latitude = latitude;
	this.longitude = longitude;
	this.widgetName = widgetName;
	this.map = null;
	this.markers = new Array() ;
	this.mapControl = null;
	this.authToken = null;
	this.showLayerControl = true;
	this.showOnlyLastTag = false;
	// Listeners map 
	// each map element is an Array of function()
	this.eventListeners = new Object();

	
	this.initMapWidget();
}

MapWidget.prototype.setShowOnlyLastTag = function (showOnlyLastTag){
	this.showOnlyLastTag = showOnlyLastTag;
}

/*
* Initialize map widget
*/ 
MapWidget.prototype.initMapWidget = function (){
	// Map initialization
	this.map = L.map(this.widgetName).setView([this.latitude , this.longitude], 13);

	L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
		maxZoom: 18,
		attribution: '&copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>'
	}).addTo(this.map);
}
	
/*
 * Add tags array to the map
 * @param {array<tag>} tags
 */
MapWidget.prototype.addLoadTagsResultToMap = function ( tags){
	this.removeAllTagsFromMap();
	
	if (tags.length == 0 ) return;
 
	var tagMarkers = new Object();

	for (var i = tags.length-1 ; i >= 0 ; i--){
		var tag = tags[i];
		var userName = tag["user"];
		 
		if ( !(userName in tagMarkers) ){
			tagMarkers[userName] = new Array();
			console.log("Creating array for " + userName);
		}
		
		// If this.showOnlyLastTag == true then add only one last tag for each user
		// otherwize add all tags
		if (this.showOnlyLastTag && tagMarkers[userName].length == 0 || !this.showOnlyLastTag){
			var currentMarker = L.marker([tag.latitude, tag.longitude], {icon: this.icon}).
			bindPopup(DataMark.getStringRepresentation(tag));
			this.markers.push(currentMarker);
			tagMarkers[userName].push(currentMarker);
		}
		
	}
	
	this.addLayerControl(tagMarkers);
}

/* 
 * Creates UI element with checkbox for each user
 * @param {map layer - tag array} tagMarkers
 */
MapWidget.prototype.addLayerControl = function (tagMarkers){

	overlayMaps = new Object();
	
	for (var u in tagMarkers){
		console.log(u+ " " + tagMarkers[u].length);
		var currentGroup = L.layerGroup(tagMarkers[u]).addTo(this.map);
		overlayMaps[u] = currentGroup;	
	} 
	if (this.showLayerControl) this.mapControl = L.control.layers(null, overlayMaps).addTo(this.map);
}
	
/*
 * Remove all tags from map
 */
MapWidget.prototype.removeAllTagsFromMap = function(){
	if (this.markers.length == 0  ) return;
	for (var i = 0 ; i<this.markers.length; i++){
		this.map.removeLayer(this.markers[i]);
	}
	this.markers.length=0;

	if (this.showLayerControl) this.map.removeControl(this.mapControl);
}	

/*
 * Centers map in (this.latitude, this.longitude)
 */
MapWidget.prototype.centerInDefaultPosition = function(){

	this.map.panTo(new L.LatLng(this.latitude, this.longitude));
}
	
/*
 * Change (this.latitude, this.longitude) and call centerInDefaultPosition
 */
MapWidget.prototype.changeMapCenter = function (latitude, longitude){
	this.latitude = latitude;
	this.longitude = longitude;
	this.centerInDefaultPosition();
}	
	
MapWidget.prototype.login = function (userName, password){
	sendLoginRequest(userName, password,
		bind(this, "onLoginSuccess"), bind(this, "onErrorOccured"));
}	
	
/*
 * Load tags for coordinates this.latitude, this.longitude and this.radius 
 */
MapWidget.prototype.loadTags = function (latitude, longitude, radius){

	if (this.authToken == null) return;

	sendLoadTagsRequest(this.authToken, latitude, longitude, radius, 
		bind(this, "onLoadTagsSuccess"), bind(this, "onErrorOccured"));
		
};

/*
 * Perform filterCircle request
 */
MapWidget.prototype.filterCircle = function (latitude, longitude, radius, timeFrom, timeTo, tagNumber){

	if (this.authToken == null) return;

	sendFilterCircleRequest(this.authToken, latitude, longitude, timeFrom, timeTo, radius, null, tagNumber,
		bind(this, "onFilterSuccess"), bind(this, "onErrorOccured"));	
		
}

/*
 * Add channels array to the map
 * @param {array<channel>} channels
 */
MapWidget.prototype.addFilterResultsToMap = function (channels){
	this.removeAllTagsFromMap();
	
	if (channels.length == 0 ) return;
 
	var tagMarkers = new Object();

	for (var i =0 ; i < channels.length ; i++){
	
		var tags = channels[i].channel.items;
		var channelName = channels[i].channel.name;
		tagMarkers[channelName] = new Array();
		
		var tagAmount = ((this.showOnlyLastTag==true) ? tags.length-1  : 0);
		console.log(this.showOnlyLastTag+" "+channelName+" "+i+" "+tagAmount);
		
		for (var j = tags.length-1; j >= tagAmount; j--){
			
			var tag = tags[j];
			tag["channel"] = channelName;
			var currentMarker = L.marker([tag.latitude, tag.longitude], {icon: this.icon}).
			bindPopup(DataMark.getStringRepresentation(tag));
			currentMarker.pubDate = tag.pubDate;
			
			this.markers.push(currentMarker);
		
			tagMarkers[channelName].push(currentMarker);
		}
	}
	
	console.log("this.markers.length =" + this.markers.length);
	
	this.addLayerControl(tagMarkers);
};

MapWidget.prototype.findLastMarker = function (){
	var lastMarker = null;
	if (this.markers.length != 0 ){
		console.log("mapWidget.markers.length != 0 ");
		
		var lastDate = dateToInt(this.markers[0].pubDate);
		var iLast = 0;
		for (var i=1; i<this.markers.length; i++){
			var currentDate = dateToInt(this.markers[i].pubDate);
			console.log(this.markers[iLast].pubDate+"|"+this.markers[i].pubDate);
			if (currentDate > lastDate){
				lastDate = currentDate;
				iLast = i;
				console.log("New last element found" + i);
			}	
		}
		console.log("Last date = " + this.markers[iLast].pubDate);
		var lastMarker = this.markers[iLast];
	}
	
	return lastMarker;
}


MapWidget.prototype.onFilterSuccess = function (jsonResponse){

	this.addFilterResultsToMap(jsonResponse.channels);
	var lastMarker = this.findLastMarker();
	if (lastMarker != null ){
		lastMarker.openPopup();
	}
	this.raiseEvent("onFilterSuccess");
}	
		
/*
 * Store auth_token and trigger onLoginSuccess event
 */
MapWidget.prototype.onLoginSuccess = function (jsonResponse){
	this.authToken = jsonResponse.auth_token;
	console.log("LoginQuery succed");
	
	this.raiseEvent("onLoginSuccess");
}



MapWidget.prototype.onLoadTagsSuccess = function (jsonResponse){

	var tags = jsonResponse.rss.channels.items[0].items;
	console.log("onLoadTagsSuccess: Loaded "+ tags.length + " tags");
	this.addLoadTagsResultToMap(tags);
}
	
MapWidget.prototype.onErrorOccured = function (jsonResponse){
	this.raiseEvent("onErrorOccured");
	console.log("onErrorOccured");
	//alert("Error during requests processing, errno = "+jsonResponse.errno);
}


/* 
 * Add listener to event
 * @param {string} event
 * @param {function()} listener
 */
MapWidget.prototype.addEventListener = function (event, listener){
	var listeners;
	if (event in this.eventListeners)
		listeners = this.eventListeners[event];
	else 
		listeners = new Array();
	
	if (listeners.indexOf(listener) == -1)
		listeners.push(listener);
	
	this.eventListeners[event] = listeners;
}

/*
 * Call all listeners for event
 * @param {string} event
 */
MapWidget.prototype.raiseEvent = function(event){
	if (event in this.eventListeners){
		var listeners = this.eventListeners[event];
		for (var i=0; i<listeners.length ; i++){
			console.log("Executing "+i+" listener");
			(listeners[i])();
		}
	}else{
		console.log("Event " + event+ " not found at MapWidget!!!");
	}
}


/*
 * Hack for passing methods as function arguments
 * TODO add var_arg support for general usage
 */
function bind(toObject, methodName){
    return function(jsonResponse){toObject[methodName](jsonResponse)}
}
