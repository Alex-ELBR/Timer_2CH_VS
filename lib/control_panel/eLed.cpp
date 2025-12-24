#include <eLed.h>

enum LED_STATE
{
    LED_OFF,
    LED_ON,    
    LED_BLINK,
};

ELed::ELed(GPIO_TypeDef *port, uint16_t pin)
{
    eled_port = port;
    eled_pin = pin;
    
    state_led = LED_OFF;
    periodic();
}

void ELed::on(void) {state_led = LED_ON;}
void ELed::off(void) {state_led = LED_OFF;}
void ELed::blink(void) {state_led = LED_BLINK;}

void ELed::toogle(void)
{
    if(toggle_led) { on();}
    else { off();}
    toggle_led ^= true;
}


void ELed::periodic(void)
{
    switch(state_led)
    {
        case LED_OFF:
        {
            HAL_GPIO_WritePin(eled_port, eled_pin, GPIO_PIN_RESET);
        }; break;

        case LED_ON:
        {
            HAL_GPIO_WritePin(eled_port, eled_pin, GPIO_PIN_SET);
        }; break;

        case LED_BLINK:
        {
            HAL_GPIO_TogglePin(eled_port, eled_pin);
        }; break;
    }
}