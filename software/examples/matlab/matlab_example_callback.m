function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialDual020mAV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    id020 = handle(BrickletIndustrialDual020mAV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register current callback to function cb_current
    set(id020, 'CurrentCallback', @(h, e) cb_current(e));

    % Set period for current (channel 0) callback to 1s (1000ms) without a threshold
    id020.setCurrentCallbackConfiguration(0, 1000, false, 'x', 0, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for current callback
function cb_current(e)
    fprintf('Channel: %i\n', e.channel);
    fprintf('Current: %g mA\n', e.current/1000000.0);
    fprintf('\n');
end
