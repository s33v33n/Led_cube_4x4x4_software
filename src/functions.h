/* Functions are desired to steer the led cube. Led cube is built of 16 columns and 4 layers */

#include <Arduino.h>

void led_cube_pins_setup(void);
void initial_effect(void);

void columns_turn_off(void); // Turn off all columns -> write state HIGH 
void columns_turn_on(void); // Turn on all columns -> write state LOW (GND)

void all_layers_low(void);  // Turn off all layers 

namespace random_numbers_generator {

    uint8_t *return_random_numbers_0_15(void);
}

namespace writing_cols_states{
    
    void write_selected_cols_states(uint8_t*, uint8_t, bool);
}
  

void write_next_layer(uint8_t);

void turn_on_one_column(uint8_t);

void turn_one_led (uint8_t);

void fountian(uint8_t*, uint8_t, uint8_t);
