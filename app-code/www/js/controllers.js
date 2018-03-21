angular.module('smartbin.controllers', [])

.controller('AppCtrl', function($scope, $ionicModal, $timeout) {

  // With the new view caching in Ionic, Controllers are only called
  // when they are recreated or on app start, instead of every page change.
  // To listen for when this page is active (for example, to refresh data),
  // listen for the $ionicView.enter event:
  //$scope.$on('$ionicView.enter', function(e) {
  //});

  // Form data for the login modal
  $scope.loginData = {};

  // Create the login modal that we will use later
  $ionicModal.fromTemplateUrl('templates/login.html', {
    scope: $scope
  }).then(function(modal) {
    $scope.modal = modal;
  });

  // Triggered in the login modal to close it
  $scope.closeLogin = function() {
    $scope.modal.hide();
  };

  // Open the login modal
  $scope.login = function() {
    $scope.modal.show();
  };

  // Perform the login action when the user submits the login form
  $scope.doLogin = function() {
    console.log('Doing login', $scope.loginData);

    // Simulate a login delay. Remove this and replace with your login
    // code if using a login system
    $timeout(function() {
      $scope.closeLogin();
    }, 1000);
  };
})

.controller('HomeCtrl', function($scope, $stateParams, $state,$ionicPopup) {
 $scope.frcount="";
  $scope.ctcount="";
  $scope.permit=false;
  $scope.iconUrl='img/spin.gif';
  //Enable Location
  $scope.EnableLocation = function() {
      try{
        var permissions = cordova.plugins.permissions;
        permissions.requestPermission(permissions.ACCESS_FINE_LOCATION, success, error);
         console.log('in Permission');
        function error() {
          console.log('ACCESS_FINE_LOCATION permission is not turned on');
        }
         
        function success( status ) {
          if( !status.hasPermission )
          {
            error();
          } else{
            $scope.permit=true;
            console.log('has Permission');
            $scope.$apply();
          }
        }
    }catch(e){
      alert(e);
    }  
  }

//Init - Check Permissions, Assign header image, get app version, get Centers count.
  //too much of jobs.. can be reduced by moving into other pages. or making LazyCalls
  $scope.Init = function() {
    
      ionic.Platform.ready(function(){
        $scope.headerImage = 'img/menu-bg.jpg';

        try{
          var permissions = cordova.plugins.permissions;
            permissions.checkPermission(permissions.ACCESS_FINE_LOCATION, function( status ){
              if ( status.hasPermission ) {
                console.log("Yes :D ");
                $scope.permit=true;//for status button on home screen
              }
              else {
                console.warn("No :( ");
                $scope.permit=false
              }
            });
          }catch(e){
            alert(e);
          }

          try{
            //cordova.getAppVersion.getVersionNumber().then(function (version) {
                //$('.version').text(version);
            //}); 
          }catch(e){
            console.log(e);
          }
      });

  }

})
.controller('MapviewCtrl', function($scope,$ionicLoading, $stateParams, $state,NgMap,$rootScope,BinService,$ionicPopup) {
    $scope.bin={};
    $scope.position={lat:null,lon:null}; //Very trouble some
    $scope.length='';
    $scope.status='';
  $scope.Init = function(){

    //$ionicLoading.show({
     // template: '<img src="img/spin.gif" > <br>Getting Bins..'
    //});

      //Get center count
      BinService.getBins().success(function(data) {
          console.log(data);
          $scope.bin=data;


        }).error(function(data) {
          var alertPopup = $ionicPopup.alert({
          title: 'Fetch Failed',
          template: 'Please check your your internet connection!'
        });
      });


    $scope.status='Getting location...<ion-spinner></ion-spinner>';
      try
      {
        var onLocationSuccess = function(position) {
            lat = position.coords.latitude;
            lon = position.coords.longitude;
            $scope.position = {lat:position.coords.latitude,lon:position.coords.longitude};
          NgMap.getMap().then(function(map) {
            $rootScope.map = map;
            $rootScope.logLatLng = function(e) {
              console.log('loc', e.latLng);
            }
            $rootScope.wayPoints = [
              {location: {lat:26.8796822, lng: 75.812079}, stopover: true},
              {location: {lat:26.8814972, lng: 75.8113023}, stopover: true},
              {location: {lat:26.8831576, lng:75.8100745},stopover: true}
            ];

            var total;
            console.log(map.directionsRenderers[0].directions);
            //var myroute = map.directionsRenderers[0].directions.routes[0];
            //for (var i = 0; i < myroute.legs.length; i++) {
            //  total += myroute.legs[i].distance.value;
            //}
            //total = map.directionsRenderers[0].directions.routes[0].legs[0].distance.value / 1000;
            //$scope.length = total;
          });
          $scope.status='Location locked: '+position.coords.latitude+','+position.coords.longitude;
        };
        function onLocationError(error) {
            alert('code: '    + error.code    + '\n' +
                  'message: ' + error.message + '\n');
        }
        navigator.geolocation.getCurrentPosition(onLocationSuccess, onLocationError);   
      }catch(e){
        console.log(e);
      }

      
  }

})
.controller('PlaylistsCtrl', function($scope) {
  $scope.playlists = [
    { title: 'Reggae', id: 1 },
    { title: 'Chill', id: 2 },
    { title: 'Dubstep', id: 3 },
    { title: 'Indie', id: 4 },
    { title: 'Rap', id: 5 },
    { title: 'Cowbell', id: 6 }
  ];
})

.controller('PlaylistCtrl', function($scope, $stateParams) {
});
