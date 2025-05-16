#include <stdio.h>
#include "pico/stdlib.h"
#include "gps_uart.h"

int main() {
    stdio_init_all();

    GPSUART gps(uart0, 9600);  // GPS modules usually default to 9600 baud

    char nmea[128];

    while (true) {
        if (gps.readNMEASentence(nmea, sizeof(nmea))) {
            printf("NMEA: %s\n", nmea);
        }
        sleep_ms(1000);
    }

    return 0;
}
