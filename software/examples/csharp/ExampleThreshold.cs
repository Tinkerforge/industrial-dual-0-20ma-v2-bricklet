using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

	// Callback function for voltage callback
	static void CurrentCB(BrickletIndustrialDual020mAV2 sender, byte channel, int current)
	{
		Console.WriteLine("Channel: " + channel);
		Console.WriteLine("Current: " + current/1000000.0 + " mA");
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDual020mAV2 id020 =
		  new BrickletIndustrialDual020mAV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register voltage callback to function CurrentCB
		id020.CurrentCallback += CurrentCB;

		// Configure threshold for current (channel 0) "greater than 10 mA"
		// with a debounce period of 10s (10000ms)
		id020.SetCurrentCallbackConfiguration(0, 10000, false, '>', 10*1000000, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
