/*------------------------------ 
* Project: 1000 HPK
* Author: DaranaSV
* Contact: darana.sv@gmail.com
* Created at: 28/12/2016
------------------------------ */
jQuery(document).ready(function() {

	jQuery('#preloader').delay(500).fadeOut(500);
	
    domready(function() {
        "use strict";
        selectnav('mainmenu', {
            label: '',
            nested: true,
            indent: '-'
        });
    });

    jQuery("#testi-carousel").owlCarousel({
        singleItem: true,
        lazyLoad: true,
        navigation: false
    });
    jQuery(".custom-carousel-1").owlCarousel({
        items: 3,
        navigation: false,
        pagination: false,
    });
    jQuery(".custom-carousel-2").owlCarousel({
        items: 3,
        navigation: false,
        pagination: false,
    });
	// wow jquery
	new WOW().init();

    $window = jQuery(window);
    jQuery('section[data-type="background"]').each(function() {
        var $bgobj = jQuery(this);
        jQuery(window).scroll(function() {
            anim();
            var yPos = -($window.scrollTop() / $bgobj.data(
                'speed'));
            var coords = '50% ' + yPos + 'px';
            $bgobj.css({
                backgroundPosition: coords
            });
        });
    });
    document.createElement("article");
    document.createElement("section");
    
    jQuery(".fx .desc").fadeTo(0, 0);
    jQuery(".fx .item").hover(function() {
            speed = 700;
            jQuery(this).find(".desc").stop(true).animate({
                'height': "120px",
                'margin-top': "20px",
                "opacity": "100"
            }, speed, 'easeOutCubic');
            jQuery(this).find(".overlay").stop(true).animate({
                'height': "100%",
                'margin-top': "20px"
            }, speed, 'easeOutCubic');
            jQuery(this).parent().parent().find(".item").not(this).stop(
                true).fadeTo(speed, .5);
        }, function() {
            jQuery(this).find(".desc").stop(true).animate({
                'height': "0px",
                'margin-top': "0px",
                "opacity": "0"
            }, speed, 'easeOutCubic');
            jQuery(this).find(".overlay").stop(true).animate({
                'height': "84px",
                'margin-top': "20px"
            }, speed, 'easeOutCubic');
            jQuery(this).parent().parent().find(".item").not(this).stop(
                true).fadeTo(speed, 1);
        })

    var revapi;
    revapi = jQuery('#revolution-slider').revolution({
        delay: 15000,
        startwidth: 1170,
        startheight: 500,
        hideThumbs: 10,
        fullWidth: "off",
        fullScreen: "on",
        fullScreenOffsetContainer: "",
        touchenabled: "on",
        navigationType: "none",
    });
    jQuery().UItoTop({
        easingType: 'easeInOutExpo'
    });

    jQuery('.callbacks_nav').hide();
    jQuery('#slider').hover(function() {
        jQuery('.callbacks_nav').stop().animate({
            opacity: 1
        }, 100);
    }, function() {
        jQuery('.callbacks_nav').stop().animate({
            opacity: 0
        });
    }, 100);
    
    jQuery(".pic-hover .hover").hide();
    jQuery(".pic-hover").hover(function() {
        jQuery(this).find(".hover").width(jQuery(this).find(
            "img").css("width"));
        jQuery(this).find(".hover").height(jQuery(this).find(
            "img").css("height"));
        jQuery(this).find(".hover").fadeTo(150, .9);
        picheight = jQuery(this).find("img").css("height");
        newheight = (picheight.substring(0, picheight.length -
            2) / 2);
        jQuery(this).find(".btn-view-details").css({
            'margin-top': newheight
        }, 'fast');
    }, function() {
        jQuery(this).find(".hover").fadeTo(150, 0);
    })
    jQuery(function() {
        jQuery(".pic_slider").responsiveSlides({
            auto: true,
            pager: false,
            nav: true,
            speed: 500,
            namespace: "callbacks",
            before: function() {
                jQuery('.events').append(
                    "<li>before event fired.</li>"
                );
            },
            after: function() {
                jQuery('.events').append(
                    "<li>after event fired.</li>"
                );
            }
        });
    });
    
    jQuery('.lt_tab').find('.lt_tab_content div').hide();
    jQuery('.lt_tab').find('.lt_tab_content div:first').show();
    jQuery('.lt_nav li').click(function() {
        jQuery(this).parent().find('li span').removeClass(
            "active");
        jQuery(this).find('span').addClass("active");
        jQuery(this).parent().parent().find(
            '.lt_tab_content div').hide();
        var indexer = jQuery(this).index();
        jQuery(this).parent().parent().find(
            '.lt_tab_content div:eq(' + indexer + ')').fadeIn();
    });
});
jQuery(document).ready(function() {
    var $header = jQuery("header"),
        $clone = $header.before($header.clone().addClass("clone"));
    jQuery(window).on("scroll", function() {
        var fromTop = jQuery(window).scrollTop();
        jQuery("body").toggleClass("down", (fromTop > 240));
    });
});