#include <Arduino.h>
#include <SPI.h>

// #define TRANSMITTER // Else, RECEIVER

#ifdef TRANSMITTER
#include "tx.hpp"
#else
#include "rx.hpp"
#endif
