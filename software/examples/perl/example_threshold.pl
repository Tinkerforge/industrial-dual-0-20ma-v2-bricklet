#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDual020mAV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

# Callback subroutine for voltage callback
sub cb_current
{
    my ($channel, $current) = @_;

    print "Channel: $channel\n";
    print "Current: " . $current/1000000.0 . " mA\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $id020 = Tinkerforge::BrickletIndustrialDual020mAV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register voltage callback to subroutine cb_current
$id020->register_callback($id020->CALLBACK_CURRENT, 'cb_current');

# Configure threshold for current (channel 0) "greater than 10 mA"
# with a debounce period of 10s (10000ms)
$id020->set_current_callback_configuration(0, 10000, 0, '>', 10*1000000, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
