'use strict';

// FIXME: This keys should be hidden (why not make a simple API in Parse cloud code?)
const MANIFEST = chrome.runtime.getManifest();
const PARSE_URL = "https://api.parse.com/1/";
const HEADER = {
        "X-Parse-Application-Id": MANIFEST.parse_app_id,
        "X-Parse-REST-API-Key": MANIFEST.parse_api_key,
        "Content-Type": "application/json"
    };

/*
 * Start a service worker to register for push notifications, which will produce a token.
 * We'll make sure to store the user's token into Parse to send notifications later.
 */
if ('serviceWorker' in navigator) {
    console.log('Service Worker is supported');
    navigator.serviceWorker.register('/sw.js').then(function(reg) {
        reg.pushManager.subscribe({
            userVisibleOnly: true
        }).then(function(sub) {
            console.log('endpoint:', sub.endpoint);
            HttpRequest(
                PARSE_URL + "installations",
                "POST",
                HEADER,
                JSON.stringify({
                    "GCMSenderId": MANIFEST.gcm_sender_id, // https://developers.google.com/cloud-messaging
                    "deviceType": "android", // Works on browsers as well
                    "pushType" : "gcm",
                    "deviceToken": sub.endpoint.replace("https://android.googleapis.com/gcm/send/", "")
                }),
                function print(resp) { console.log(resp); }
            );
        });
    }).catch(function(error) {
        console.error(':^(', error);
    });
}

/* -- UTILS -- */

/**
 * Makes an asynchronous request
 * @method requestData
 * @param  {String}    url
 * @param  {String}    method
 * @param  {Array}     header
 * @param  {String}    data
 * @param  {Function}  callback success
 */
function HttpRequest(url, method, header, data, callback){
    // compatible with IE7+, Firefox, Chrome, Opera, Safari
    var xmlhttp = new XMLHttpRequest(), key;
    xmlhttp.onreadystatechange = function(){
        if (xmlhttp.readyState == 4 && xmlhttp.status == 201){
            callback && callback(xmlhttp.responseText);
        }
    }
    xmlhttp.open(method, url, true);
    for (key in header) {
        xmlhttp.setRequestHeader(key, header[key]);
    };
    xmlhttp.send(data);
}
