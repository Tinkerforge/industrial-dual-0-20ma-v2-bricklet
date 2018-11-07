use std::{error::Error, io, thread};
use tinkerforge::{industrial_dual_0_20ma_v2_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let id020 = IndustrialDual020maV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let current_receiver = id020.get_current_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `id020` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for current in current_receiver {
            println!("Channel: {}", current.channel);
            println!("Current: {} mA", current.current as f32 / 1000000.0);
            println!();
        }
    });

    // Configure threshold for current (channel 0) "greater than 10 mA"
    // with a debounce period of 10s (10000ms).
    id020.set_current_callback_configuration(0, 10000, false, '>', 10 * 1000000, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
