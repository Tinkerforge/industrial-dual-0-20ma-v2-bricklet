use std::{error::Error, io, thread};
use tinkerforge::{industrial_dual_0_20ma_v2_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let industrial_dual_0_20ma_v2_bricklet = IndustrialDual020mAV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for voltage events.
    let current_listener = industrial_dual_0_20ma_v2_bricklet.get_current_receiver();
    // Spawn thread to handle received events. This thread ends when the industrial_dual_0_20ma_v2_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in current_listener {
            println!("Channel: {}", event.channel);
            println!("Current: {}{}", event.current as f32 / 1000000.0, " mA");
            println!();
        }
    });

    // Configure threshold for current (channel 0) "greater than 10 mA"
    // with a debounce period of 10s (10000ms)
    industrial_dual_0_20ma_v2_bricklet.set_current_callback_configuration(0, 10000, false, '>', 10 * 1000000, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
