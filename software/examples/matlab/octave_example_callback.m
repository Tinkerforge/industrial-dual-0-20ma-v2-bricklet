function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    id020 = javaObject("com.tinkerforge.BrickletIndustrialDual020mAV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register current callback to function cb_current
    id020.addCurrentCallback(@cb_current);

    % Set period for current (channel 0) callback to 1s (1000ms) without a threshold
    id020.setCurrentCallbackConfiguration(0, 1000, false, "x", 0, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for current callback
function cb_current(e)
    fprintf("Channel: %d\n", e.channel);
    fprintf("Current: %g mA\n", e.current/1000000.0);
    fprintf("\n");
end
