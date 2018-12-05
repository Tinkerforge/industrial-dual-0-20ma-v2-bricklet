use std::{io, error::Error};
use std::thread;
use tinkerforge::{ip_connection::IpConnection, 
                  industrial_dual_0_20ma_v2_bricklet::*};


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
		println!("Current: {} mA", current.current as f32 /1000000.0);
		println!();
            }
        });

		// Set period for current (channel 0) callback to 1s (1000ms) without a threshold.
		id020.set_current_callback_configuration(0, 1000, false, 'x', 0, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
