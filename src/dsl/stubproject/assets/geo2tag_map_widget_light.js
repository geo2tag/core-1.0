function MapWidget(latitude, longitude,  widgetName){
	this.latitude = latitude;
	this.longitude = longitude;
	this.widgetName = widgetName;
	this.map = null;
	this.markers = new Array() ;
	this.mapControl = null;
	this.authToken = null;
	this.markerChannelMap = new Object();
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
	

MapWidget.prototype.addTag = function (user, channel, title, description, link, latitude, longitude, pubDate){

	var tag	= {
		channel: channel,
		title: title,
		description: description,
		user: user,
		link: link,
		pubDate: pubDate
	};

	var currentMarker = L.marker([latitude, longitude]).
	bindPopup(DataMark.getStringRepresentation(tag));
	this.markers.push(currentMarker);

	this.updateLayerControl(channel, currentMarker);	

}


MapWidget.prototype.updateLayerControl = function (channel, marker){


	if (!(channel in this.markerChannelMap))
		this.markerChannelMap[channel] = new Array();

	this.markerChannelMap[channel].push(marker);

	if (this.mapControl != null)
		this.map.removeControl(this.mapControl);

	this.addLayerControl(this.markerChannelMap);
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
