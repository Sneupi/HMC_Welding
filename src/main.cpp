#include <Arduino.h>
#include <SPI.h>

#define RX

#if defined(TX)
#include "tx.hpp"
#elif defined(RX)
#include "rx.hpp"
#elif defined(CTRL)
#include "ctrl.hpp"
#else
#error "Please define a role: TX, RX or CTRL"
#endif
