#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_industrial_dual_0_20ma_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
id020 = BrickletIndustrialDual020mAV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current current from channel 0
voltage = id020.get_current 0
puts "Voltage (Channel 0): #{voltage/1000000.0} mA"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
