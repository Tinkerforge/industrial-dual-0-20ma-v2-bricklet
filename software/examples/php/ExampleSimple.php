<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDual020mAV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDual020mAV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$id020 = new BrickletIndustrialDual020mAV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current current from channel 0
$current = $id020->getCurrent(0);
echo "Current (Channel 0): " . $current/1000000.0 . " mA\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
