function openNav() {
    $(".navbarToggle").toggleClass("toggled");
    $("#main-aside").toggleClass("open");
}

$(document).ready(function() {
    if (!navigator.cookieEnabled) {
        $(".main-container").hide();
        $(".no-cookies").show();
    }
})