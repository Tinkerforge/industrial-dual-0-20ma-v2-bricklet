// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_industrial_dual_0_20ma_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

// Callback function for current callback
static void current_handler(TF_IndustrialDual020mAV2 *device, uint8_t channel,
                            int32_t current, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Channel: %I8u\n", channel);
	tf_hal_printf("Current: %d 1/%d mA\n", current, 1000000);
	tf_hal_printf("\n");
}

static TF_IndustrialDual020mAV2 id020;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_industrial_dual_0_20ma_v2_create(&id020, NULL, hal), "create device object");

	// Register current callback to function current_handler
	tf_industrial_dual_0_20ma_v2_register_current_callback(&id020,
	                                                       current_handler,
	                                                       NULL);

	// Configure threshold for current (channel 0) "greater than 10 mA"
	// with a debounce period of 10s (10000ms)
	tf_industrial_dual_0_20ma_v2_set_current_callback_configuration(&id020, 0, 10000, false, '>', 10*1000000, 0);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
