function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    id020 = javaObject("com.tinkerforge.BrickletIndustrialDual020mAV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register voltage callback to function cb_current
    id020.addCurrentCallback(@cb_current);

    % Configure threshold for current (channel 0) "greater than 10 mA"
    % with a debounce period of 10s (10000ms)
    id020.setCurrentCallbackConfiguration(0, 10000, false, ">", 10*1000000, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for voltage callback
function cb_current(e)
    fprintf("Channel: %d\n", e.channel);
    fprintf("Current: %g mA\n", e.current/1000000.0);
    fprintf("\n");
end
