#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDual020mAV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $id020 = Tinkerforge::BrickletIndustrialDual020mAV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current current from channel 0
my $voltage = $id020->get_current(0);
print "Voltage (Channel 0): " . $voltage/1000000.0 . " mA\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
