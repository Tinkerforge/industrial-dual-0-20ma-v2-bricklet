import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDual020mAV2;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDual020mAV2 id020 =
		  new BrickletIndustrialDual020mAV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add current listener
		id020.addCurrentListener(new BrickletIndustrialDual020mAV2.CurrentListener() {
			public void current(int channel, int current) {
				System.out.println("Channel: " + channel);
				System.out.println("Current: " + current/1000000.0 + " mA");
				System.out.println("");
			}
		});

		// Set period for current (channel 0) callback to 1s (1000ms) without a threshold
		id020.setCurrentCallbackConfiguration(0, 1000, false, 'x', 0, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
