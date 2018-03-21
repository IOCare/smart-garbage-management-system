// Ionic Starter App

// angular.module is a global place for creating, registering and retrieving Angular modules
// 'starter' is the name of this angular module example (also set in a <body> attribute in index.html)
// the 2nd parameter is an array of 'requires'
// 'starter.controllers' is found in controllers.js
var lat;
var lon;
angular.module('smartbin', ['ionic', 'smartbin.controllers','smartbin.services','ngMap'])

.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {
    // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
    // for form inputs)

window.plugins.OneSignal
  .startInit("3ca6aedf-01e3-460c-815a-1cddd831eab8")
  .handleNotificationOpened(function(jsonData) {
    alert("Notification opened:\n" + JSON.stringify(jsonData));
    console.log('didOpenRemoteNotificationCallBack: ' + JSON.stringify(jsonData));   
  })
  .endInit();
  
    try
    {
      if (cordova.platformId == 'android') {
          StatusBar.backgroundColorByHexString("#ffe065");
      }

      var onLocationSuccess = function(position) {
          console.log('Latitude: '          + position.coords.latitude          + '\n' +
                'Longitude: '         + position.coords.longitude         + '\n' +
                'Altitude: '          + position.coords.altitude          + '\n' +
                'Accuracy: '          + position.coords.accuracy          + '\n' +
                'Altitude Accuracy: ' + position.coords.altitudeAccuracy  + '\n' +
                'Heading: '           + position.coords.heading           + '\n' +
                'Speed: '             + position.coords.speed             + '\n' +
                'Timestamp: '         + position.timestamp                + '\n');
          lat = position.coords.latitude;
          lon = position.coords.longitude;
      };
      function onLocationError(error) {
          alert('code: '    + error.code    + '\n' +
                'message: ' + error.message + '\n');
      }
      navigator.geolocation.getCurrentPosition(onLocationSuccess, onLocationError);   
    }catch(e){
      console.log(e);
    }





    if (window.cordova && window.cordova.plugins.Keyboard) {
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
      cordova.plugins.Keyboard.disableScroll(true);

    }
    if (window.StatusBar) {
      // org.apache.cordova.statusbar required
      StatusBar.styleDefault();
    }
  });
})

.config(function($stateProvider, $urlRouterProvider) {
  $stateProvider

    .state('app', {
    url: '/app',
    abstract: true,
    templateUrl: 'templates/menu.html',
    controller: 'AppCtrl'
  })

  .state('app.mapview', {
    url: '/mapview',
    views: {
      'menuContent': {
        templateUrl: 'templates/mapview.html',
        controller: 'MapviewCtrl'
      }
    }
  })

  .state('app.home', {
      url: '/home',
      views: {
        'menuContent': {
          templateUrl: 'templates/home.html',
          controller: 'HomeCtrl'
        }
      }
    })
    .state('app.playlists', {
      url: '/playlists',
      views: {
        'menuContent': {
          templateUrl: 'templates/playlists.html',
          controller: 'PlaylistsCtrl'
        }
      }
    })

  .state('app.single', {
    url: '/playlists/:playlistId',
    views: {
      'menuContent': {
        templateUrl: 'templates/playlist.html',
        controller: 'PlaylistCtrl'
      }
    }
  });
  // if none of the above states are matched, use this as the fallback
  $urlRouterProvider.otherwise('/app/home');
});
