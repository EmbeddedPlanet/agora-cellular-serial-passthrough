
#include "mbed.h"
 
RawSerial  pc(PIN_NAME_DEBUG_TX, PIN_NAME_DEBUG_RX, 115200);
RawSerial  device(PIN_NAME_CELL_TX, PIN_NAME_CELL_RX, 115200);

DigitalOut sensor_power_enable(PIN_NAME_SENSOR_POWER_ENABLE);
DigitalOut cell_power(PIN_NAME_CELL_POWER_ENABLE);
DigitalOut led(LED1);
 
void dev_recv()
{
    while(device.readable()) {
        pc.putc(device.getc());
    }
}
 
void pc_recv()
{
    led = !led;
    while(pc.readable()) {
        char c = pc.getc();
        //pc.putc(c);
        device.putc(c);
    }
}

int main()
{
    device.set_flow_control(SerialBase::RTSCTS, PIN_NAME_CELL_RTS, PIN_NAME_CELL_CTS);

    // Enable power to the sensors
    pc.puts("Enabling sensor power...\r\n");
    sensor_power_enable = 1;

    // Enable power to the cell module
    pc.puts("Enabling cell power...\r\n");
    cell_power = 1;

    // Initialize mbedTLS
    pc.puts("Setting up cryptocell...\r\n");
    mbedtls_platform_setup(NULL);

    pc.puts("Turn on cell module...\r\n");
    gpio_t gpio;

    gpio_init_out_ex(&gpio, PIN_NAME_CELL_ON_OFF, 1);
    wait_ms(500);
    gpio_write(&gpio, 0);
    wait_ms(5000);
    gpio_write(&gpio, 1);
    wait_ms(5000);
 
    pc.puts("Entering while loop...\r\n");
 
    pc.attach(&pc_recv, Serial::RxIrq);
    device.attach(&dev_recv, Serial::RxIrq);
 
    while(1) {
        sleep();
    }
}
