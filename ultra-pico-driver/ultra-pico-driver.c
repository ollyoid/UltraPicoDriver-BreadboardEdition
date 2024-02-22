#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/pwm.h"
#include <string.h>

// Assembled PIO program:
#include "phase.pio.h"

# define CHANNELS_PER_PIN 16
# define PIN_COUNT 1

// Define the pins SER = serial data, SRCLK = shift register Clock, RCLK = storage register clock.
// SER is only the first pin - phase_PIN_COUNT consecutive pins will be set up.
#define SER 18
#define SRCLK 17
#define RCLK 16

volatile uint8_t phase_vals[CHANNELS_PER_PIN * PIN_COUNT];
volatile uint8_t amp_vals[CHANNELS_PER_PIN * PIN_COUNT];
volatile uint waves[CHANNELS_PER_PIN * PIN_COUNT];

char string[100];

typedef enum {DRIVER_STATE_IDLE, DRIVER_STATE_ACTIVE} driver_state;

driver_state state;
// The pio and dma configs need to be made global in order for the DMA interrupt to re-initialise
// them.
PIO pio;
int dma_channel;
dma_channel_config dma_config;
uint sm;

// The DMA interrupt handler to set up DMA again after a whole wave (32 samples) has been sent to the 
void dma_handler() {
    // Unset the DMA IRQ bit
    dma_hw->ints0 = 1u << dma_channel;
    // Restart the DMA
    dma_channel_configure(dma_channel, &dma_config, &pio->txf[sm], &waves, CHANNELS_PER_PIN * PIN_COUNT, true);
}


// Takes the shift and amp values and uses bit manipulation to populate the wave table.
void populate_waves(){
    // Clear array first
    for (int i = 0; i < CHANNELS_PER_PIN*PIN_COUNT; i++){
        waves[i] = 0;
    }
    // Go through each bit in array setting it individually (slow but easy to understand)
    uint index = 0; // The index of the bit in the array we are setting
    for(uint sample = 0; sample <32; sample ++) { // 32 samples per cycle (resolution of pi/16)
        for (uint channel = 0; channel < CHANNELS_PER_PIN; channel++) { // channels are map tp the physical transducers
            for (int pin = 0; pin < PIN_COUNT; pin ++) { // each serial data pin
                // Array is made up of 32 bit words
                uint word = index/32;
                uint bit = index%32;
                // Based on the phase shift, and which sample in the cycle, is the wave high or low?
                uint start = 15;
                // Offset based on experimentally tested linear relationship between duty cycle and phase shift
                // Correct when powered with external PSU at 18V
                start += (15-amp_vals[channel*PIN_COUNT+pin]);
                uint end = start + amp_vals[channel*PIN_COUNT+pin];
                uint position = ((sample+phase_vals[channel*PIN_COUNT+pin])%32);
                uint logic_level = (position > start) && (position < end);
                waves[word] |= logic_level << bit;
                index ++;
            }
        }
    }
}

// Set a PWM pin to act as the Storage register clock pin, this is used to both to apply the shift
// register values to the outputs, and to synchronize the state machines.
void init_rclk(){
    gpio_set_function(RCLK, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(RCLK);
    uint channel_num =  pwm_gpio_to_channel(RCLK);
    pwm_set_wrap(slice_num, 99);
    pwm_set_chan_level(slice_num, channel_num, 5);
    pwm_set_enabled(slice_num, true);
}   


void init_pio(){

    // Assign functions to the pins.
    for (int p = 0; p < PIN_COUNT; p++) {
        gpio_set_function(SER+p, GPIO_FUNC_PIO0);
    }
    gpio_set_function(SRCLK, GPIO_FUNC_PIO0);

    pio = pio0;
    uint offset = pio_add_program(pio, &phase_program);
    sm = pio_claim_unused_sm(pio, true);
    pio_sm_set_consecutive_pindirs(pio, sm, SRCLK, 1+ PIN_COUNT, true);

    pio_sm_config config = phase_program_get_default_config(offset);

    // Define the serial pins as out pins
    sm_config_set_in_pins(&config, RCLK);
    sm_config_set_out_pins(&config, SER, PIN_COUNT);
    sm_config_set_sideset(&config, 1, false, false);
    // Define the clock pin as a sideset for the pio program
    sm_config_set_sideset_pins(&config, SRCLK);
    sm_config_set_out_shift(&config, true, true, 0);

    // Initialise the state machine
    pio_sm_init(pio, sm, offset, &config);

    //set the clock divider (We can afford to do this, it just reduces high speed signals and we don't have to deal with them)
    pio_sm_set_clkdiv(pio, sm, 2.0);

    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}

void init_dma() {
    dma_channel = dma_claim_unused_channel(true);

    // Enable the IRQ handler
    dma_channel_set_irq0_enabled(dma_channel, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    // Configure the channel
    dma_config = dma_channel_get_default_config(dma_channel);
    channel_config_set_dreq(&dma_config, pio_get_dreq(pio, sm, true));
    channel_config_set_read_increment(&dma_config, true);
    channel_config_set_write_increment(&dma_config, false);
    channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_32);
    dma_channel_configure(dma_channel, &dma_config, &pio->txf[sm], &waves, CHANNELS_PER_PIN * PIN_COUNT, true);
}


// Buffer -> (split in two) -> PIO -> Pumped onto pin -> PCB output 
uint8_t transducer_mapping[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
uint8_t map_transducers(uint n) {
    return transducer_mapping[n];
}


int read_until(char c, char *buffer, uint max){
    for (int i = 0; i < max; i ++){
        char r = getchar();
        // putchar(r);
        if (r == c) {
            buffer[i] = 0;
            return 0;
        } else {
            buffer[i] = r;
        }
    }
    return 1;
}

void read_phase_vals(){
    for (int i = 0; i < 16; i ++) {
        read_until(',', string, 99);
        phase_vals[map_transducers(i)] = atoi(string);
    }
    printf("New Phase Vals:\n[");
    for (int i = 0; i < 15; i ++) {
        printf("%d,", phase_vals[map_transducers(i)]);
    }
    printf("%d]\n", phase_vals[15]);
}

void read_amp_vals(){
    for (int i = 0; i < 16; i ++) {
        read_until(',', string, 99);
        amp_vals[map_transducers(i)] = atoi(string);
    }
    printf("New Amp Vals:\n[");
    for (int i = 0; i < 15; i ++) {
        printf("%d,", amp_vals[map_transducers(i)]);
    }
    printf("%d]\n", amp_vals[15]);
}

int main()
{
    set_sys_clock_khz(128000, true);
    stdio_init_all();
    stdio_flush();


    init_rclk();
    init_pio();
    init_dma();

    for (int i = 0; i < 16; i ++) {
        amp_vals[i] = 15;
    } 

    const uint LED_PIN = 21;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    state = DRIVER_STATE_ACTIVE;
    gpio_put(LED_PIN, 1);

    while (true)
    {   
        populate_waves();
        char r = getchar();
        if (r == 254) 
        {
            for (int p = 0; p < 16; p++) {
                char b = getchar();
                // if (r < 31) {
                    phase_vals[map_transducers(p)] = b;
                // }
                // else {
                //     printf("Phase value out of range\n");
                // }
            }
        }
        else if (r== 251)
        {
            for (int a = 0; a < 16; a++) {
                char c = getchar();
                // if (r < 15) {
                amp_vals[map_transducers(a)] = c;
                // }
                // else {
                //     printf("Amplitude value out of range\n");
                // }
            }
        } else {
            printf("\nUnrecognised Command\n");
        }
    }
    return 0;
}
