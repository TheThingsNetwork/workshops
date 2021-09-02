function decodeUplink(input) {
  var data = {};
  data.sensorValue = (input.bytes[0] << 8) + input.bytes[1];
  var warnings = [];
  return {
    data: data,
    warnings: warnings
  };
}
