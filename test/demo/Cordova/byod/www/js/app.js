// Ionic Starter App

// angular.module is a global place for creating, registering and retrieving Angular modules
// 'starter' is the name of this angular module example (also set in a <body> attribute in index.html)
// the 2nd parameter is an array of 'requires'
// 'starter.controllers' is found in controllers.js
angular.module('starter', ['ionic', 'starter.controllers'])

.run(function($ionicPlatform, $location, $rootScope, $ionicPopup) {
  $ionicPlatform.ready(function() {
    // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
    // for form inputs)
//    if (window.cordova && window.cordova.plugins.Keyboard) {
//      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
//    }
//    if (window.StatusBar) {
//      // org.apache.cordova.statusbar required
//      StatusBar.styleDefault();
//    }
	  
	  $rootScope.rootWorkPath = cordova.file.applicationStorageDirectory;

	  if ($rootScope.rootWorkPath.indexOf('file://') === 0) {
           $rootScope.rootWorkPath = $rootScope.rootWorkPath.substring(7);
	  }
                       
      if ($rootScope.rootWorkPath.indexOf('localhost') === 0) {

           $rootScope.rootWorkPath = $rootScope.rootWorkPath.substring(9);
      }
                       
      $rootScope.rootWorkPath += "Documents/CordovaDemo/";
      
      $rootScope.demoPath = cordova.file.externalRootDirectory; 
      
      if ($rootScope.demoPath) {
    	  if ($rootScope.demoPath.indexOf('file://') === 0) {
              $rootScope.demoPath = $rootScope.demoPath.substring(7);
    	  }
                          
         if ($rootScope.demoPath.indexOf('localhost') === 0) {

              $rootScope.demoPath = $rootScope.demoPath.substring(9);
         }
         
         $rootScope.demoPath += "CordovaDemo/";
      } else {
    	  $rootScope.demoPath = $rootScope.rootWorkPath;
      }
      
	  $rootScope.$apply();
	  
  	  window.addEventListener("netstatus", onStatusChange, false);	
  	  
	  	function onStatusChange(info) {
			if (info.newStatus !== 1) {
				var alertPopup = $ionicPopup.alert({
				     title: 'status change to ' + info.newStatus + ", interfaces can not do right!"
				});
			}
		}
  });
})

.config(function($stateProvider, $urlRouterProvider) {
  $stateProvider

  .state('login', {
	url: "/login",
	templateUrl: "login.html",
	controller: 'loginCtrl'
  })
  
  .state('FAQs', {
    url: '/FAQs',
    templateUrl: 'templates/FAQs.html',
    controller: 'FAQsCtrl'
  })
  
  .state('app', {
    url: "/app",
    abstract: true,
    templateUrl: "templates/menu.html",
    controller: 'AppCtrl'
  })

    .state('app.playlists', {
      url: "/playlists",
      views: {
        'menuContent': {
          templateUrl: "templates/playlists.html",
          controller: 'PlaylistsCtrl'
        }
      }
    })

  .state('app.getNetStatus', {
	  url: "/getStatus",
	  views: {
		  'menuContent': {
			  templateUrl: "templates/getStatus.html",
			  controller: 'statusCtrl'
		  }
	  }
  })  
  
  .state('app.encryptFile', {
	  url: "/encryptFile",
	  views: {
		  'menuContent': {
			  templateUrl: "templates/encryptFile.html",
			  controller: 'encryptFileCtrl'
		  }
	  }
  })
  
  .state('app.decryptFile', {
	  url: "/decryptFile",
	  views: {
		  'menuContent': {
			  templateUrl: "templates/decryptFile.html",
			  controller: 'decryptFileCtrl'
		  }
	  }
  })
  .state('app.viewEncryptedFile', {
	  url: "/viewEncryptedFile",
	  views: {
		  'menuContent': {
			  templateUrl: "templates/viewEncryptedFile.html",
			  controller: 'viewEncryptedFileCtrl'
		  }
	  }
  })
  
  .state('app.webView', {
	  url: "/webView",
	  views: {
		  'menuContent': {
			  templateUrl: "templates/setWebViewUseSvn.html",
			  controller: 'webViewCtrl'
		  }
	  }
  })
  
  
   .state('app.videoPlay', {
	  url: "/videoPlay",
	  views: {
		  'menuContent': {
			  templateUrl: "templates/videoplay.html",
			  controller: 'videoPlayCtrl'
		  }
	  }
  })
  
  .state('app.single', {
    url: "/playlists/:playlistId",
    views: {
      'menuContent': {
        templateUrl: "templates/playlist.html",
        controller: 'PlaylistCtrl'
      }
    }
  });
  
  // if none of the above states are matched, use this as the fallback
  $urlRouterProvider.otherwise('/login');
});
