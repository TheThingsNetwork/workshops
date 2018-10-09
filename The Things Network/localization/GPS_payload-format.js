function Decoder(bytes, port) {
  var lat = ((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]) / 1000000;
  var lon = ((bytes[4] << 24) | (bytes[5] << 16) | (bytes[6] << 8) | bytes[7]) / 1000000;

  return {
    lat: lat,
    lon: lon,
  }
}
