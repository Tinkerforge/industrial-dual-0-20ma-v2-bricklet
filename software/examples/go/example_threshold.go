package main

import (
	"fmt"
	"tinkerforge/industrial_dual_0_20ma_v2_bricklet"
	"tinkerforge/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	id020, _ := industrial_dual_0_20ma_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	id020.RegisterCurrentCallback(func(channel uint8, current int32) {
		fmt.Printf("Channel: %d\n", channel)
		fmt.Printf("Current: %d mA\n", float64(current)/1000000.0)
		fmt.Println()
	})

	// Configure threshold for current (channel 0) "greater than 10 mA"
	// with a debounce period of 10s (10000ms).
	id020.SetCurrentCallbackConfiguration(0, 10000, false, '>', 10*1000000, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
