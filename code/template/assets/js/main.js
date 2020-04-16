(function ($) {
    "use strict";

    /*--
        Commons Variables
    -----------------------------------*/
    var $window = $(window),
        $body = $('body'),
        $mainWrapper = $('.main-wrapper'),
        $headerHeight = $('.header').outerHeight();

    /*--
        Custom script to call Background
        Image form html data attribute
    -----------------------------------*/
    $('[data-bg-image]').each(function () {
        var $this = $(this),
            $image = $this.data('bg-image');
        $this.css('background-image', 'url(' + $image + ')');

    });

    /*--
        Parallax
    -----------------------------------*/
    $('.bg-parallax').each(function () {
        $(this).parallax("50%", 0.5);
    })

    /*--
        Header Sticky
    -----------------------------------*/
    $window.on('scroll', function () {
        if ($window.scrollTop() > 350) {
            $('.header').addClass('is-sticky');
        } else {
            $('.header').removeClass('is-sticky');
        }
    });

    /*--
		Mobile OffCanvas Open & Close
    -----------------------------------*/
    function mobileOffCanvasToggle() {
        var $offCanvas = $('#offcanvas'),
            $offCanvasOverlay = $('.offcanvas-overlay');
        $('.offcanvas-toggle').on('click', function () {
            $offCanvas.addClass('open');
            $offCanvasOverlay.fadeIn();
            $body.addClass('offcanvas-open');
        });
        $('.offcanvas-close, .offcanvas-overlay').on('click', function () {
            $offCanvas.removeClass('open');
            $offCanvasOverlay.fadeOut();
            $body.removeClass('offcanvas-open');
        });
    }
    mobileOffCanvasToggle();

    /*--
        Off Canvas Menu
    -----------------------------------*/
    function mobileOffCanvasMenu() {
        var $offCanvasNav = $('.offcanvas-menu'),
            $offCanvasNavSubMenu = $offCanvasNav.find('.sub-menu');

        /*Add Toggle Button With Off Canvas Sub Menu*/
        $offCanvasNavSubMenu.parent().prepend('<span class="menu-expand"><i></i></span>');

        /*Category Sub Menu Toggle*/
        $offCanvasNav.on('click', 'li a, li .menu-expand', function (e) {
            var $this = $(this);
            if ($this.siblings('.sub-menu').length && ($this.attr('href') === '#' || $this.hasClass('menu-expand'))) {
                e.preventDefault();
                if ($this.siblings('ul:visible').length) {
                    $this.parent('li').removeClass('active');
                    $this.siblings('ul').slideUp();
                    $this.parent('li').find('li').removeClass('active');
                    $this.parent('li').find('ul:visible').slideUp();
                } else {
                    $this.parent('li').addClass('active');
                    $this.closest('li').siblings('li').removeClass('active').find('li').removeClass('active');
                    $this.closest('li').siblings('li').find('ul:visible').slideUp();
                    $this.siblings('ul').slideDown();
                }
            }
        });
    }
    mobileOffCanvasMenu();

    /*--
        Login & Register Modal
    -----------------------------------*/
    $('#loginSignupModal').on('show.bs.modal', function (event) {
        var $button = $(event.relatedTarget),
            $target = $button.data('target-sub'),
            $modal = $(this);
        $modal.find('.nav-link').removeClass('active');
        $modal.find('.nav-link[href="' + $target + '"]').addClass('active');
        $modal.find('.tab-pane').removeClass('active show');
        $modal.find('.tab-pane' + $target).addClass('active show');
    })

    /*--
        Slick Slider Activation
    -----------------------------------*/

    // Company Slider
    $('.company-slider').slick({
        slidesToShow: 6,
        slidesToScroll: 1,
        arrows: true,
        autoplay: true,
        prevArrow: '<button class="slick-prev"><i class="fa fa-angle-left"></i></button>',
        nextArrow: '<button class="slick-next"><i class="fa fa-angle-right"></i></button>',
        responsive: [{
            breakpoint: 1200,
            settings: {
                slidesToShow: 5
            }
        }, {
            breakpoint: 992,
            settings: {
                slidesToShow: 4
            }
        }, {
            breakpoint: 767,
            settings: {
                slidesToShow: 3,
                arrows: false
            }
        }, {
            breakpoint: 480,
            settings: {
                slidesToShow: 2,
                arrows: false
            }
        }]
    });

    // Job Category Slider
    $('.job-category-slider').slick({
        slidesToShow: 5,
        slidesToScroll: 1,
        arrows: true,
        autoplay: true,
        prevArrow: '<button class="slick-prev"><i class="fa fa-angle-left"></i></button>',
        nextArrow: '<button class="slick-next"><i class="fa fa-angle-right"></i></button>',
        responsive: [{
            breakpoint: 1200,
            settings: {
                slidesToShow: 4
            }
        }, {
            breakpoint: 992,
            settings: {
                slidesToShow: 3
            }
        }, {
            breakpoint: 767,
            settings: {
                slidesToShow: 2,
                arrows: false
            }
        }, {
            breakpoint: 480,
            settings: {
                slidesToShow: 1
            }
        }]
    });

    // Testimonial Slider
    $('.testimonial-slider').slick({
        slidesToShow: 1,
        slidesToScroll: 1,
        arrows: true,
        autoplay: true,
        prevArrow: '<button class="slick-prev"><i class="fa fa-angle-left"></i></button>',
        nextArrow: '<button class="slick-next"><i class="fa fa-angle-right"></i></button>',
        responsive: [{
            breakpoint: 767,
            settings: {
                slidesToShow: 1,
                arrows: false
            }
        }]
    });

    // Blog Slider
    $('.blog-slider').slick({
        slidesToShow: 3,
        slidesToScroll: 1,
        arrows: false,
        autoplay: true,
        prevArrow: '<button class="slick-prev"><i class="fa fa-angle-left"></i></button>',
        nextArrow: '<button class="slick-next"><i class="fa fa-angle-right"></i></button>',
        responsive: [{
            breakpoint: 992,
            settings: {
                slidesToShow: 2
            }
        }, {
            breakpoint: 767,
            settings: {
                slidesToShow: 2,
                arrows: false
            }
        }, {
            breakpoint: 767,
            settings: {
                slidesToShow: 2,
                arrows: false
            }
        }, {
            breakpoint: 575,
            settings: {
                slidesToShow: 1
            }
        }]
    });

    /*--
        Counter Up
    -----------------------------------*/
    $('.counter').counterUp({
        time: 3000
    });

    /*--
        Salary Range (Ion Range Slider)
    -----------------------------------*/
    $("#salary-range").ionRangeSlider({
        type: "double",
        min: 3000,
        max: 25000,
        step: 500,
        from: 7000,
        to: 15000,
        grid: false,
    });

    /*--
        MailChimp
    -----------------------------------*/
    $('#mc-form').ajaxChimp({
        language: 'en',
        callback: mailChimpResponse,
        // ADD YOUR MAILCHIMP URL BELOW HERE!
        url: 'http://devitems.us11.list-manage.com/subscribe/post?u=6bbb9b6f5827bd842d9640c82&amp;id=05d85f18ef'

    });

    function mailChimpResponse(resp) {
        if (resp.result === 'success') {
            $('.mailchimp-success').html('' + resp.msg).fadeIn(900);
            $('.mailchimp-error').fadeOut(400);
        } else if (resp.result === 'error') {
            $('.mailchimp-error').html('' + resp.msg).fadeIn(900);
        }
    }

    /*--
        Scroll Up
    -----------------------------------*/
    $.scrollUp({
        scrollText: '<i class="fa fa-long-arrow-up"></i>',
    });

    /*--
        Google Map
    -----------------------------------*/
    $('.google-map').each(function () {
        if ($(this).length) {
            var $this = $(this),
                $id = $this.attr('id'),
                $zoom = $this.data('zoom') ? $this.data('zoom') : 12,
                $lat = $this.data('lat') ? $this.data('lat') : 21.0277214,
                $long = $this.data('long') ? $this.data('long') : 105.8342015;

            function initialize() {
                var mapOptions = {
                    zoom: $zoom,
                    scrollwheel: false,
                    center: new google.maps.LatLng($lat, $long),
                };
                var map = new google.maps.Map(document.getElementById($id), mapOptions);
                var marker = new google.maps.Marker({
                    position: map.getCenter(),
                    map: map
                    //animation: google.maps.Animation.BOUNCE
                });
            }
            google.maps.event.addDomListener(window, 'load', initialize);
        }
    });

    /*--
        Ajax Contact Form
    -----------------------------------*/
    $(function () {
        // Get the form.
        var form = $('#contact-form');
        // Get the messages div.
        var formMessages = $('.form-messege');
        // Set up an event listener for the contact form.
        $(form).submit(function (e) {
            // Stop the browser from submitting the form.
            e.preventDefault();
            // Serialize the form data.
            var formData = $(form).serialize();
            // Submit the form using AJAX.
            $.ajax({
                    type: 'POST',
                    url: $(form).attr('action'),
                    data: formData
                })
                .done(function (response) {
                    // Make sure that the formMessages div has the 'success' class.
                    formMessages.removeClass('error text-danger').addClass('success text-success mt-3').text(response);
                    // Clear the form.
                    form.find('input:not([type="submit"]), textarea').val('');
                })
                .fail(function (data) {
                    // Make sure that the formMessages div has the 'error' class.
                    formMessages.removeClass('success text-success').addClass('error text-danger mt-3');
                    // Set the message text.
                    if (data.responseText !== '') {
                        formMessages.text(data.responseText);
                    } else {
                        formMessages.text('Oops! An error occured and your message could not be sent.');
                    }
                });
        });
    });

})(jQuery);