// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_industrial_dual_0_20ma_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


static TF_IndustrialDual020mAV2 id020;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_industrial_dual_0_20ma_v2_create(&id020, UID, hal), "create device object");

	// Get current current from channel 0
	int32_t current;
	check(tf_industrial_dual_0_20ma_v2_get_current(&id020, 0,
	                                               &current), "get current from channel 0");

	tf_hal_printf("Current (Channel 0): %d 1/%d mA\n", current, 1000000);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
