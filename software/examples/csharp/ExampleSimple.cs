using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDual020mAV2 id020 =
		  new BrickletIndustrialDual020mAV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current current from channel 0
		int voltage = id020.GetCurrent(0);
		Console.WriteLine("Voltage (Channel 0): " + voltage/1000000.0 + " mA");

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
