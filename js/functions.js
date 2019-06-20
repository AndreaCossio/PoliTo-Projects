function openNav() {
    $(".navbarToggle").toggleClass("toggled");
    $("#main-aside").toggleClass("open");
}

function validate() {
    var email = $("#register .input-container").first().children("input");
    if (/^[a-zA-Z0-9\.\!\#\$\%\&\'\*\+\/\=\?\^\_\`\{\|\}\~\-]+@[a-zA-z0-9\-]+\.[a-zA-z0-9]+$/.test(email.val())) {
        return true;
    } else {
        var span = document.createElement("span");
        $(span).text("The email is not valid.").insertAfter($(".form-title h1"));
        return false;
    }
}

$(document).ready(function() {
    if (!navigator.cookieEnabled) {
        $(".main-container").hide();
        $(".no-cookies").show();
    }
})