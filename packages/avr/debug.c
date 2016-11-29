/*
 * eChronos Real-Time Operating System
 * Copyright (C) 2016 National ICT Australia Limited (NICTA), ABN 62 102 206 173.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3, provided that these additional
 * terms apply under section 7:
 *
 *   No right, title or interest in or to any trade mark, service mark, logo
 *   or trade name of of National ICT Australia Limited, ABN 62 102 206 173
 *   ("NICTA") or its licensors is granted. Modified versions of the Program
 *   must be plainly marked as such, and must not be distributed using
 *   "eChronos" as a trade mark or product name, or misrepresented as being
 *   the original Program.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @TAG(NICTA_AGPL)
 */
#include <stdbool.h>
#include <avr/io.h>
#if defined (__AVR_ATmega128__)
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
#include <util/setbaud.h>
#else
#error unsupported hardware
#endif

static void uart_init(void);

void
debug_puts(const char *c)
{
    static bool uart_is_initialized;

    if (!uart_is_initialized)
    {
        uart_init();
        uart_is_initialized = true;
    }

    while (*c)
    {
#if defined (__AVR_ATmega128__)
        /* unfortunately, this breaks older versions of simulavr which we rely on for system tests */
        /* loop_until_bit_is_set(UCSR0A, UDRE); */
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
        loop_until_bit_is_set(UCSR0A, UDRE0);
#else
#error unsupported hardware
#endif
        UDR0 = *c;
        c += 1;
    }
}

static void uart_init(void)
{
#if defined (__AVR_ATmega128__)
    UCSR0B = (1 << RXEN) | (1 << TXEN);
    UCSR0C = (1 << UCSZ0) | (1 << UCSZ1);
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    UCSR0A &= ~(_BV(U2X0));
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
#else
#error unsupported hardware
#endif
}
