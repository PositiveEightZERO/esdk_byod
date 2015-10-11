angular.module('starter.controllers', ['ionic'])

.controller('loginCtrl', function($scope,$rootScope, $state,$ionicPopup, $timeout) {
	
	// Form data for the login modal
	  $scope.loginData = {
	      "gateway" : "10.170.119.67",
	      "username" : "lzy",
	      "password" : "Admin@123"
	  };
	  
	  $scope.loginBtn = {
	      "value" : "Log In",
	      "id" : "login"
	  };
	  
	  $scope.username = {
	      "label" : "Username:"
	  };
	  
	  $scope.password = {
	      "label" : "Password:"
	  };
	  
	  $scope.gateway = {
	      "label" : "Gateway:"
	  };
	  
	  $scope.workPath = {
	      "label" : "WorkPath:"
	  };
	
	  function loginSuccess(res) {
		  var loginPopup = $ionicPopup.show({
			  title : 'login result is:' + res
		  });
		  $timeout(function(){
			  loginPopup.close();
			  
			  $("#login").removeAttr("disabled");
			  $("#login")[0].innerText = "Log In";
			  $state.go("app.playlists");
		  }, 800);
	  }
	  
	  function loginError(res) {
		  var loginPopup = $ionicPopup.show({
			  title : 'login failed, errorCode is:' + res
		  });
		  
		  $timeout(function(){
			  loginPopup.close();
			  $("#login").removeAttr("disabled");
			  $("#login")[0].innerText = "Log In";
		  }, 800);
	  }
	  
	  function initSuccess(res) {
		  var initPopup = $ionicPopup.show({
			  title : 'init result is:' + res
		  });
		  $timeout(function() {
			  initPopup.close();
			  
			  AnyOffice.LoginAgent.login(loginSuccess,loginError,$scope.loginData);
			  
		  }, 1000);
	  }
	  
	  function initError() {
		  var initPopup = $ionicPopup.show({
			  title : 'init error'
		  });
		  
		  $timeout(function() {
			  initPopup.close();
			  
		  }, 1000);
	  }
	  
	  
	  // Perform the login action when the user submits the login form
	  $scope.doLogin = function() {
		  var pop = null;
		  //登录参数检查
		  if ($scope.loginData.gateway === "") {
			  pop = $ionicPopup.alert({
				  title : 'gateway can not be null!'
			  });
			  
			  $timeout(function() {
				  pop.close(); //close the popup after 3 seconds for some reason
			  }, 1000);
			  
			  return;
		  }
		  
		  if ($scope.loginData.username === "") {
			  pop = $ionicPopup.alert({
				  title : 'username can not be null!'
			  });
			  
			  $timeout(function() {
				  pop.close(); //close the popup after 3 seconds for some reason
			  }, 1000);
			  
			  return;
		  }
		  
		  if ($scope.loginData.password === "") {
			  pop = $ionicPopup.alert({
				  title : 'password can not be null!'
			  });
			  
			  $timeout(function() {
				  pop.close(); //close the popup after 3 seconds for some reason
			  }, 1000);
			  
			  return;
		  }
		  $("#login")[0].innerText = "Logging...";
		  $("#login").attr('disabled','true');
		  
		  $scope.loginData.workPath = $rootScope.rootWorkPath;
		  
		  //初始化sdk
		  AnyOffice.SDKContext.init(initSuccess, initError, $scope.loginData);
		  
	  };
	  
	  $scope.encrypt = function() {
		 var innerText = $("#encrypt")[0].innerText;
		 console.log(innerText);
		 if (innerText === "Encrypt") {
			 $("#encrypt").removeClass("ion-locked");
			 $("#encrypt").addClass("ion-unlocked");
			 $("#encrypt")[0].innerText = "Decrypt";
		 } else {
			 $("#encrypt").removeClass("ion-unlocked");
			 $("#encrypt").addClass("ion-locked");
			 $("#encrypt")[0].innerText = "Encrypt";
		 }
		 
	  }
	  
	  $scope.goFAQs = function() {
		  $state.go("FAQs");
	  }
})

.controller('FAQsCtrl', function($scope, $state) {
	$scope.goLogin = function() {
		$state.go("login");
	}
})

.controller('AppCtrl', function($scope, $ionicModal, $timeout) {
  
})

.controller('PlaylistsCtrl', function($scope, $state) {
  $scope.playlists = [
    { title: '网络状态管理', id: "netStatus", list : [{title: '获取网络状态', id : "getNetStatus"}]},
    { title: 'Webview接口', id: "webview", list : [{title:"webview访问", id: "webView"}] },
    //{ title: '在线视频播放', id: "videoplay", list : [{title:"视频播放", id: "videoPlay"}] },
    { title: '文档', id: "file", list : [{title: "文档加密", id : "encryptFile"}, {title: "文档解密", id : "decryptFile"}, {title: "文档浏览", id : "viewEncryptedFile"}] }
  ];
  
  $scope.goSingle = function(id) {
	   $state.go("app." + id);
  }
})

.controller('statusCtrl', function($scope, $ionicPopup, $timeout) {
	$scope.showResult = false;
	$scope.statusResult = "";
	
	var searchPopup = null;
	
	var status = {
		"0" : "未连接",
		"1" : "已连接",
		"2" : "连接中"
	};
	
	function success(res) {
		$timeout(function(){
			if (searchPopup) {
				searchPopup.close();
			}
			$scope.showResult = true;
			$scope.statusResult = "查询返回:" + res + ",当前网络状态为:" + status[res];
		}, 2000);
	}
	
	function fail() {
		
	}
	
	$scope.getNetStatus = function() {
		$scope.showResult = false;
		$scope.statusResult = "";
		AnyOffice.NetStatusManager.getNetStatus(success, fail);
		searchPopup = $ionicPopup.show({
			title : 'searching...'
	    });
	}
})

.controller('encryptFileCtrl', function($rootScope, $scope, $ionicPopup, $timeout, $ionicModal, $state, $ionicScrollDelegate) {
	$scope.showBack = false;
	
	var desDir = $rootScope.demoPath + "Encrypt/aa.jpg";
	$scope.encryptData = {
		fileName : "http://172.22.8.206:8080/HttpServerDemo/e/Download.do?fileName=aa.jpg",
		desFileName : desDir
	};
	
	$scope.$watch('encryptData.fileName',
	    function(to, from){
        	var i = to.lastIndexOf("=");
        	if (i > 0) {
    			var fname = to.substr(i + 1);
    			desDir = desDir.substr(0, desDir.lastIndexOf("/")) + "/" + fname;
    			$scope.encryptData.desFileName = desDir;
    		} 
        }
    );
	
	function success(res) {
		if (res.loaded) {
			$scope.downloadInfo = "downloading.........." + res.loaded;
			if (res.lengthComputable && res.lengthComputable === "true") {
				$scope.downloadInfo = $scope.downloadInfo + "/" + res.total;
			}
		} else {
			console.log(JSON.stringify(res));
			$scope.downloadInfo = 'download done!go to dest dir to see the file!'
		}
		
		$scope.$apply();
	}
	
	function fail(res) {
		var alertPopup = $ionicPopup.alert({
		     title: "Error",
		     template: JSON.stringify(res)
		});
	}
	
	$scope.doEncrypt = function() {
		if ($scope.encryptData.fileName === "" || $scope.encryptData.desFileName === "") {
			var errorPopup = $ionicPopup.alert({
				title : '路径不能为空'
			});
			return;
		}
		AnyOffice.FileEncryption.fileEncryptDownload(success, fail, {"source": encodeURI($scope.encryptData.fileName), "target" : $scope.encryptData.desFileName, "objectId":"1" });
	}
	
	$scope.abort = function() {
		AnyOffice.FileEncryption.abortDownload(abortSuccess, abortFail, {"objectId":"1" });
	}
	
	function abortSuccess () {
		$scope.downloadInfo = "Abort done!!";
		$scope.$apply();
	}
	
	function abortFail (res) {
		console.log(JSON.stringify(res));
	}
})

.controller('decryptFileCtrl', function($rootScope, $scope, $ionicPopup, $timeout, $ionicModal, $state, $ionicScrollDelegate) {
	
	$scope.showBack = false;
	
	var desDir = $rootScope.demoPath + "Decrypt";
	$scope.decryptData = {
		fileName : "",
		desFileName : desDir
	}
	
	$scope.goBack = function() {
		if (!$scope.showBack) {
			$scope.modal.hide();
			return;
		}
		
		var dir = $scope.rootDir.substring(0, $scope.rootDir.lastIndexOf("/"));
		if (dir === "") {
			dir = $rootScope.demoPath;
		}
		AnyOffice.FilePlugin.listFile(listSuccess, listFail, {dirName: dir});
	}
	
	$scope.doDecrypt = function() {
		if ($scope.decryptData.fileName === "" || $scope.decryptData.desFileName === "") {
			var errorPopup = $ionicPopup.alert({
				title : '路径不能为空'
			});
			return;
		}
		
		AnyOffice.FileEncryption.fileDecrypt(success, fail, {"srcFileName": $scope.decryptData.fileName, "dstFileName" : $scope.decryptData.desFileName});
	}
	
	$scope.rootDir = $rootScope.demoPath;
	
	$scope.files = [];
	
	$scope.clickItem = function(file) {
		if (file.isDirectory) {
			AnyOffice.FilePlugin.listFile(listSuccess, listFail, {dirName: file.fullPath});
			$scope.rootDir = file.fullPath;
		} else {
			$scope.decryptData.fileName = file.fullPath;
			$scope.decryptData.desFileName = desDir + file.fullPath.substring(file.fullPath.lastIndexOf("/"));
			$scope.modal.hide();
		}
	};
	
	function listSuccess (ret) {
		$ionicScrollDelegate.scrollTop();
		$scope.$apply(function() {
			$scope.files = ret;
			if (ret.length > 0) {
				$scope.rootDir = ret[0].parentPath;
			}
			$scope.showBack = ($scope.rootDir + "/" !== $rootScope.demoPath);
		});
	}
	
	function listFail () {
		
	}
	
	$ionicModal.fromTemplateUrl('fileExplore.html', function(modal){
		$scope.modal = modal;
	}, {
		scope: $scope,
		animation: 'slide-in-up'
	});
	
	$scope.openFileDialog = function() {
		$scope.modal.show();
	}
	
	  $scope.$on('modal.hidden', function() {
		  
	  });
	
	function success() {
		var successPopup = $ionicPopup.show({
			title : 'decryption done!go to dest path to see the file!'
		});
		$timeout(function(){
			successPopup.close();
		}, 3000);
	}
	
	function fail(res) {
		alert(res);
	}
	
	AnyOffice.FilePlugin.listFile(listSuccess, listFail, {dirName: $rootScope.demoPath});
})

.controller('viewEncryptedFileCtrl', function($rootScope, $scope, $ionicPopup, $timeout, $ionicModal, $state, $ionicScrollDelegate) {
	
	$scope.showBack = false;
	
	$scope.decryptData = {
		fileName : ""
	}
	
	$scope.goBack = function() {
		if (!$scope.showBack) {
			$scope.modal.hide();
			return;
		}
		var dir = $scope.rootDir.substring(0, $scope.rootDir.lastIndexOf("/"));
		if (dir === "") {
			dir = $rootScope.demoPath;
		}
		AnyOffice.FilePlugin.listFile(listSuccess, listFail, {dirName: dir});
	}
	
	$scope.read = function() {
		if ($scope.decryptData.fileName === "") {
			var errorPopup = $ionicPopup.alert({
				title : '文件路径不能为空'
			});
			return;
		}
		AnyOffice.FilePlugin.readFile(success, fail, {filePath: $scope.decryptData.fileName});
	}
	
	$scope.rootDir = $rootScope.demoPath;
	
	$scope.files = [];
	
	$scope.clickItem = function(file) {
		if (file.isDirectory) {
			AnyOffice.FilePlugin.listFile(listSuccess, listFail, {dirName: file.fullPath});
			$scope.rootDir = file.fullPath;
		} else {
			$scope.decryptData.fileName = file.fullPath;
			$scope.decryptData.desFileName = $scope.decryptData.desFileName + file.fullPath.substring(file.fullPath.lastIndexOf("/"));
			$scope.modal.hide();
		}
	};
	
	function listSuccess (ret) {
		$ionicScrollDelegate.scrollTop();
		$scope.$apply(function() {
			$scope.files = ret;
			if (ret.length > 0) {
				$scope.rootDir = ret[0].parentPath;
			}
			$scope.showBack = ($scope.rootDir + "/" !== $rootScope.demoPath);
		});
	}
	
	function listFail () {
		
	}
	
	$ionicModal.fromTemplateUrl('fileExplore.html', function(modal){
		$scope.modal = modal;
	}, {
		scope: $scope,
		animation: 'slide-in-up'
	});
	
	$scope.openFileDialog = function() {
		$scope.modal.show();
	}
	
	  $scope.$on('modal.hidden', function() {
		  
	  });
	
	function success(res) {
		var successPopup = $ionicPopup.show({
				title : '读取成功'
			});
		
		$timeout(function(){
			successPopup.close();
		}, 1000);
	}
	
	function fail(res) {
		alert(res);
	}
	
	AnyOffice.FilePlugin.listFile(listSuccess, listFail, {dirName: $rootScope.demoPath});
})

.controller('webViewCtrl', function($scope) {
	$scope.visitAddress = {url : "http://172.22.8.206:8080"};
	
	$scope.doOpen = function() {
		console.log($scope.visitAddress);
		window.open($scope.visitAddress.url , "_blank", "svn=yes");

		// $.ajax(
		// {
		// 	url: "http://172.22.8.206:8180/",
		// 	cache : false,
		// 	data: "{test:0}",//提交的表单数据
		// 	type: 'post',
		// 	crossDomain: false,
		// 	dataType: 'json',
		// 	timeout: 6000000,
		// 	success: function( response ) {
		// 		alert(success);//解析返回数据并处理
		// 	},
		// 	error: function(response) {
		// 		//app.trigger("warn","提交失败。");
		// 		alert(response);
		// 	} 
		// });

	}
})

.controller('videoPlayCtrl', function($scope) {

	

    $scope.videoAddress = {url : "http://172.22.8.206:8180/test.mp4"};
            //$scope.videoAddress = {url : "http://10.170.102.180:8180/test.mp4"};
	$scope.playVideo = function() {




		console.log($scope.videoAddress);
            var remoteVideoDiv = $("#video1");
            var width = parseInt(remoteVideoDiv.width());
            var height = parseInt(remoteVideoDiv.height());
            var x = parseInt(remoteVideoDiv.offset().left);
            var y =  parseInt(remoteVideoDiv.offset().top);
            var remoteParam = {"left":x, "top":y,"width":width,"height":height };
        
		AnyOffice.VideoPlayer.play(
    		$scope.videoAddress.url,
              remoteParam,
    		{
        		volume: 0.5,
        		scalingMode: AnyOffice.VideoPlayer.SCALING_MODE.SCALE_TO_FIT_WITH_CROPPING
    		},
    		function () {
        		console.log("video completed");
    		},
    		function (err) {
        		console.log(err);
    		}
		);
	}
})


.controller('PlaylistCtrl', function($scope, $stateParams) {
});
