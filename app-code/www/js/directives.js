angular.module('bkpuneapp.directives', [])  
.directive('fbImage', [function () {
    return {
      restrict: 'A',
      link: function (scope, element, attrs) {
        element.css({
            'background-image': 'url(' + attrs.fbImage +')',
            'background-size' : 'cover',
            'background-position': 'center',
            'margin': 'auto'
        });

      }
    };
  }])
.directive('bgImage', [function () {
    return {
      restrict: 'A',
      link: function (scope, element, attrs) {

            scope.$watch('headerImage', function(newVal) {
              if (newVal) {
                element.css({
                    'background-image': 'url(' + attrs.bgImage +')',
                    'background-size' : 'cover',
                    'background-position': 'center',
                    'margin': 'auto',
                    'height': '60vw'
                });
              }
            });
      }
    };
  }])
.directive('elasticHeader', function($ionicScrollDelegate) {
    return {
        restrict: 'A',
        link: function(scope, scroller, attr) {
            var scrollerHandle = $ionicScrollDelegate.$getByHandle(attr.delegateHandle);
            var header = document.getElementById(attr.elasticHeader);
            var headerHeight = header.clientHeight;
            var translateAmt, scaleAmt, scrollTop, lastScrollTop;
            var ticking = false;
             
            // Set transform origin to top:
            header.style[ionic.CSS.TRANSFORM + 'Origin'] = 'center bottom';

            // Update header height on resize:
            window.addEventListener('resize', function() {
                headerHeight = header.clientHeight;
            }, false);
 
            scroller[0].addEventListener('scroll', requestTick);
             
            function requestTick() {
                if (!ticking) {                
                    ionic.requestAnimationFrame(updateElasticHeader);
                }
                ticking = true;
            }
             
            function updateElasticHeader() {
                 
                scrollTop = scrollerHandle.getScrollPosition().top;
             
                if (scrollTop >= 0) {
                    // Scrolling up. Header should shrink:
                    translateAmt = scrollTop / 2;
                    scaleAmt = 1;
                } else {
                    // Scrolling down. Header should expand:
                    translateAmt = 0;
                    scaleAmt = -scrollTop / headerHeight + 1;
                }
 
                // Update header with new position/size:
                header.style[ionic.CSS.TRANSFORM] = 'translate3d(0,'+translateAmt+'px,0) scale('+scaleAmt+','+scaleAmt+')';
                 
                ticking = false;
            }

        }
    }
});