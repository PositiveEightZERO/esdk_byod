
/**
 * @constructor
 */
var cordova = require('cordova'),
    exec = require('cordova/exec');

function handlers() {
  return netInfo.channels.netstatus.numHandlers;
}

var NetInfo = function() {
    this._level = null;
    this._isPlugged = null;
    this.channels = {
    	netstatus:cordova.addWindowEventHandler("netstatus")
    };
    for (var key in this.channels) {
        this.channels[key].onHasSubscribersChange = NetInfo.onHasSubscribersChange;
    }
};
NetInfo.onHasSubscribersChange = function() {
  if (this.numHandlers === 1 && handlers() === 1) {
      exec(netInfo._status, netInfo._error, "NetStatusManagerCordova", "start", []);
  } else if (handlers() === 0) {
      exec(null, null, "NetStatusManagerCordova", "stop", []);
  }
};

NetInfo.prototype._status = function (info) {

    if (info) {
        if(info.oldStatus == info.newStatus) {
            return; 
        }
        
        cordova.fireWindowEvent("netstatus", info);
    }
};

NetInfo.prototype._error = function(e) {
    console.log("Error initializing NetInfo: " + e);
};

var netInfo = new NetInfo();

module.exports = netInfo;
