function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    id020 = javaObject("com.tinkerforge.BrickletIndustrialDual020mAV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current current from channel 0
    current = id020.getCurrent(0);
    fprintf("Current (Channel 0): %g mA\n", current/1000000.0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
