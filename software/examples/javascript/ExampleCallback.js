var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var id020 = new Tinkerforge.BrickletIndustrialDual020mAV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for current (channel 0) callback to 1s (1000ms) without a threshold
        id020.setCurrentCallbackConfiguration(0, 1000, false, 'x', 0, 0);
    }
);

// Register current callback
id020.on(Tinkerforge.BrickletIndustrialDual020mAV2.CALLBACK_CURRENT,
    // Callback function for current callback
    function (channel, current) {
        console.log('Channel: ' + channel);
        console.log('Current: ' + current/1000000.0 + ' mA');
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
