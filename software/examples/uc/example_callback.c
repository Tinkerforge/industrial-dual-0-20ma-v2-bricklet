#include "bindings/hal_common.h"
#include "bindings/bricklet_industrial_dual_0_20ma_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


// Callback function for current callback
static void current_handler(TF_IndustrialDual020mAV2 *device, uint8_t channel,
                            int32_t current, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Channel: %u\n", channel);
	tf_hal_printf("Current: %d 1/%d mA\n", current, 1000000.0);
	tf_hal_printf("\n");
}

static TF_IndustrialDual020mAV2 id020;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_industrial_dual_0_20ma_v2_create(&id020, UID, hal), "create device object");

	// Register current callback to function current_handler
	tf_industrial_dual_0_20ma_v2_register_current_callback(&id020,
	                                                       current_handler,
	                                                       NULL);

	// Set period for current (channel 0) callback to 1s (1000ms) without a threshold
	tf_industrial_dual_0_20ma_v2_set_current_callback_configuration(&id020, 0, 1000, false, 'x', 0, 0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
