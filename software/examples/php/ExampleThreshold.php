<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDual020mAV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDual020mAV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

// Callback function for current callback
function cb_current($channel, $current)
{
    echo "Channel: $channel\n";
    echo "Current: " . $current/1000000.0 . " mA\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$id020 = new BrickletIndustrialDual020mAV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register current callback to function cb_current
$id020->registerCallback(BrickletIndustrialDual020mAV2::CALLBACK_CURRENT, 'cb_current');

// Configure threshold for current (channel 0) "greater than 10 mA"
// with a debounce period of 10s (10000ms)
$id020->setCurrentCallbackConfiguration(0, 10000, FALSE, '>', 10*1000000, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
