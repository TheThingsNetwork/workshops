// Decode the base64 data
var data = new Buffer(msg.payload.data, "base64").toString();

// Split the temperature and moisture
var fields = data.split(", ");

// Return the object
return {
    payload: {
        temperature: fields[0],
        moisture: fields[1]
    }
}
