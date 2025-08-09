/**
 * To prepare a new GPIO for use:
 * 1. associate the new pin to its purpose with a descriptive name.
 * 2. add this new name to AVAILABLE_GPIOs
 * 3. #define the PIN_ID corresponding to the position of the new pin in
 *    AVAILABLE_GPIOs.
 */

#ifndef gpio_pcbDetails_h
#define gpio_pcbDetails_h

#include <gpio_mcuDetails.h>

#define LED_0_PIN_ID 0

#define AVAILABLE_GPIOs {          \
    [LED_0_PIN_ID] = {GPIOC, 6},   \
}

#endif // gpio_pcbDetails_h
