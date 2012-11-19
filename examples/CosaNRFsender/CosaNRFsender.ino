/**
 * @file CosaNRFsender.ino
 * @version 1.0
 *
 * @section License
 * Copyright (C) Mikael Patel, 2012
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * @section Description
 * Cosa demonstration of NRF24L01+ driver; sender
 *
 * This file is part of the Arduino Che Cosa project.
 */

#include "Cosa/NRF.h"
#include "Cosa/Trace.h"
#include "Cosa/Watchdog.h"
#include "Cosa/Memory.h"

// NRF24L01+ Wireless communication using SPI and default pins(9, 10, 2)

NRF nrf;

// Configuration

#define USE_WATCHDOG_DELAY
// #define USE_EVENT_AWAIT

void setup()
{
  // Start trace output stream
  Trace::begin(9600);

  // Check amount of free memory
  TRACE(free_memory());
  TRACE(sizeof(NRF));

  // Start the watchdog ticks counter and push timeout events
#ifdef USE_EVENT_AWAIT
  Watchdog::begin(1024, Watchdog::push_event);
#endif
#ifdef USE_WATCHDOG_DELAY
  Watchdog::begin(64);
#endif

  // Power up the transceiver
  nrf.set_powerup_mode();

  // Check configuration and setup transmitter mode
  nrf.set_transmitter_mode("cosa1");

  // Print configuration
  TRACE(nrf.read(NRF::FEATURE));
  TRACE(nrf.read(NRF::RF_CH));
  TRACE(nrf.read(NRF::RF_SETUP));
  TRACE(nrf.read(NRF::RX_PW_P0));
  TRACE(nrf.read(NRF::RX_PW_P1));
  TRACE(nrf.read(NRF::RX_ADDR_P0));
  TRACE(nrf.read(NRF::RX_ADDR_P1));
  TRACE(nrf.read(NRF::RX_ADDR_P2));
  TRACE(nrf.read(NRF::RX_ADDR_P3));
  TRACE(nrf.read(NRF::RX_ADDR_P4));
  TRACE(nrf.read(NRF::RX_ADDR_P5));
  TRACE(nrf.read(NRF::SETUP_RETR));
  TRACE(nrf.read(NRF::SETUP_AW));
  TRACE(nrf.read(NRF::DYNPD));
  TRACE(nrf.read(NRF::CONFIG));

  // Turn off Arduino timer0 to reduce wakeups
  TIMSK0 = 0;
}

// Message block
typedef struct msg_t msg_t;
struct msg_t {
  uint16_t id; 
  uint8_t observe;
  uint8_t status;
};

msg_t msg = { 0 };
const uint8_t count = sizeof(msg);

void loop()
{
#ifdef USE_WATCHDOG_DELAY
  Watchdog::delay(1024);
#endif
#ifdef USE_EVENT_AWAIT
  Event event;
  Event::queue.await(&event);
  TRACE(event.get_type());
#endif

  // Attempt to send a message
  msg.id += 1;
  msg.status = nrf.get_status();
  msg.observe = nrf.read(NRF::OBSERVE_TX);
  if (nrf.send(&msg, count) == count) {
    Trace::print(Watchdog::get_ticks());
    Trace::print_P(PSTR(":SEND(id = "));
    Trace::print(msg.id);
    Trace::print_P(", observe = ");
    Trace::print(msg.observe, 2);
    Trace::print_P(PSTR(", status = "));
    Trace::print(msg.status, 2);
    Trace::print_P(PSTR(")\n"));
  }

  // Check if the transmission fifo needs flushing
  if (!nrf.is_ready() && nrf.is_max_retransmit()) {
    nrf.flush();
  }
}
