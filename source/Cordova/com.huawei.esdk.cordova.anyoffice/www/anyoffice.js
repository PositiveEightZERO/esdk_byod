
	var argscheck = require('cordova/argscheck'),
    	exec = require('cordova/exec');

	/**
	 * @constructor
	 */
	function AnyOffice(){
		
	}
	
	
	AnyOffice.prototype.SDKContext = {};
	AnyOffice.prototype.SDKContext.init = function(successCallback, errorCallback, options) {
		argscheck.checkArgs('fFO', 'AnyOffice.SDKContext.init', arguments);
		options = options || {};
		var getValue= argscheck.getValue;
		var workPath = getValue(options.workPath, "");
		var username = getValue(options.username, "");
		exec(successCallback, errorCallback, "SDKContextCordova", "init", [workPath, username]);
	}
	
	AnyOffice.prototype.NetStatusManager = {};
	AnyOffice.prototype.NetStatusManager.getNetStatus = function(successCallback, errorCallback) {
		argscheck.checkArgs('fF', 'AnyOffice.NetStatusManager.getNetStatus', arguments);
		exec(successCallback, errorCallback, "NetStatusManagerCordova", "getNetStatus", []);
	}
	
	AnyOffice.prototype.LoginAgent = {};
	/**
	 * LoginAgent.loginSync
	 *
	 * @param {Function} successCallback The function to call when the heading data is available
	 * @param {Function} errorCallback The function to call when there is an error getting the heading data. (OPTIONAL)
	 */
	AnyOffice.prototype.LoginAgent.login = function(successCallback, errorCallback, options) {
	    argscheck.checkArgs('fFO', 'AnyOffice.LoginAgent.login', arguments);
	    options = options || {};
	    var getValue = argscheck.getValue;

	    var username = getValue(options.username, "");
	    var password = getValue(options.password, "");
	    var gateway = getValue(options.gateway, "");
	    
	    exec(successCallback, errorCallback, "LoginAgentCordova", "login", [username, password, gateway]);
	};
	
	AnyOffice.prototype.LoginAgent.doAppAuthentication = function(successCallback, errorCallback, options) {
		argscheck.checkArgs('fFO', 'AnyOffice.LoginAgent.login', arguments);
	    options = options || {};
	    var getValue = argscheck.getValue;
	    
	    var authServerInternetAddress = getValue(options.authServerInternetAddress, "");
	    var authServerIntranetAddress = getValue(options.authServerIntranetAddress, "");
	    exec(successCallback, errorCallback, "LoginAgentCordova", "doAppAuthentication", [authServerInternetAddress, authServerIntranetAddress]);
	};
	
	AnyOffice.prototype.FileEncryption = {};
	
	AnyOffice.prototype.FileEncryption.fileEncrypt = function(successCallback, errorCallback, options) {
		argscheck.checkArgs('fFO', 'AnyOffice.FileEncryption.fileEncrypt', arguments);
	    options = options || {};
	    var getValue = argscheck.getValue;
	    
	    var srcFileName = getValue(options.srcFileName, "");
	    var dstFileName = getValue(options.dstFileName, "");
	    exec(successCallback, errorCallback, "FileEncryptionCordova", "fileEncrypt", [srcFileName, dstFileName]);
	};
	
	AnyOffice.prototype.FileEncryption.fileEncryptDownload = function(successCallback, errorCallback, options) {
		argscheck.checkArgs('fFO', 'AnyOffice.FileEncryption.fileEncryptDownload', arguments);
	    options = options || {};
	    var getValue = argscheck.getValue;
	    
	    var source = getValue(options.source, "");
	    var target = getValue(options.target, "");
	    var objectId = getValue(options.objectId, "");
	    var headers = getValue(options.headers, "");
	    exec(successCallback, errorCallback, "FileEncryptionCordova", "fileEncryptDownload", [source, target, objectId, headers]);
	};
	
	AnyOffice.prototype.FileEncryption.abortDownload = function(successCallback, errorCallback, options) {
		argscheck.checkArgs('fFO', 'AnyOffice.FileEncryption.abortDownload', arguments);
	    options = options || {};
	    var getValue = argscheck.getValue;
	    
	    var objectId = getValue(options.objectId, "");
	    exec(successCallback, errorCallback, "FileEncryptionCordova", "abortDownload", [objectId]);
	};
	
	AnyOffice.prototype.FileEncryption.fileDecrypt = function(successCallback, errorCallback, options) {
		argscheck.checkArgs('fFO', 'AnyOffice.FileEncryption.fileDecrypt', arguments);
	    options = options || {};
	    var getValue = argscheck.getValue;
	    
	    var srcFileName = getValue(options.srcFileName, "");
	    var dstFileName = getValue(options.dstFileName, "");
	    exec(successCallback, errorCallback, "FileEncryptionCordova", "fileDecrypt", [srcFileName, dstFileName]);
	};
	
	AnyOffice.prototype.FilePlugin = {};
	
	AnyOffice.prototype.FilePlugin.listFile = function(successCallback, errorCallback, options) {
		argscheck.checkArgs('fFO', 'AnyOffice.FilePlugin.listFile', arguments);
	    options = options || {};
	    var getValue = argscheck.getValue;
	    
	    var dirName = getValue(options.dirName, "");
	    exec(successCallback, errorCallback, "FilePluginCordova", "listFile", [dirName]);
	};
	
	AnyOffice.prototype.FilePlugin.readFile = function(successCallback, errorCallback, options) {
		argscheck.checkArgs('fFO', 'AnyOffice.FilePlugin.readFile', arguments);
	    options = options || {};
	    var getValue = argscheck.getValue;
	    
	    var filePath = getValue(options.filePath, "");
	    var openMode = getValue(options.openMode, "");
	    exec(successCallback, errorCallback, "FilePluginCordova", "readFile", [filePath, openMode]);
	};
	

	AnyOffice.prototype.VideoPlayer = {

		DEFAULT_OPTIONS: {
	        volume: 1.0,
	        scalingMode: 1
	    },

	    SCALING_MODE: {
	        SCALE_TO_FIT: 1,
	        SCALE_TO_FIT_WITH_CROPPING: 2
	    },

	    merge: function () {
        var obj = {};
        Array.prototype.slice.call(arguments).forEach(function(source) {
            for (var prop in source) {
                obj[prop] = source[prop];
            }
        });
        return obj;
    }
	};


    AnyOffice.prototype.VideoPlayer.play = function (path, position, options, successCallback, errorCallback) {
        options = this.merge(this.DEFAULT_OPTIONS, options);
        exec(successCallback, errorCallback, "VideoPlayerCordova", "play", [path, position, options]);
    };




	module.exports = new AnyOffice();


