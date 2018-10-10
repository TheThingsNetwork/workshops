function pad(b) {
  var h = b.toString(16);
  return (h + "").length < 2 ? "0" + h : h;
}

function Decoder(bytes, port) {
  var result = {
    access_points: []
  };
  for (var i = 0; i < bytes.length; i += 7) {
    var bssid = bytes.slice(i, i + 6).map(pad).join(":");
    var rssi = bytes[i + 6] - 0x100;
    result.access_points.push({
      bssid: bssid,
      rssi: rssi
    });
  }
  return result;
}
