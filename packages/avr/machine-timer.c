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

/*<module>
    <code_gen>template</code_gen>
    <headers>
        <header path="../rtos-example/machine-timer.h" code_gen="template" />
    </headers>
</module>*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include "machine-timer.h"

static void (*application_timer_isr_pointer)(void);

void
machine_timer_start(void (*application_timer_isr)(void))
{
    application_timer_isr_pointer = application_timer_isr;

#if defined (__AVR_ATmega128__)
    TCCR0 = 0; /* stop counter */
    TCNT0 = 0; /* reset timer counter */
    TIFR = (1 << TOV0); /* reset overflow flag */
    TIMSK |= (1 << TOIE0); /* enable interrupt on overflow */
    sei(); /* enable interrupts globally */
    TCCR0 = (1 << CS02) | (1 << CS00); /* start timer on max prescaler */
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
    TCCR0B = 0; /* stop counter */
    TCNT0 = 0; /* reset timer counter */
    TIFR0 = (1 << TOV0); /* reset overflow flag */
    TIMSK0 |= (1 << TOIE0); /* enable interrupt on overflow */
    sei(); /* enable interrupts globally */
    TCCR0B = (1 << CS02) | (1 << CS00); /* start timer on max prescaler */
#else
#error unsupported hardware
#endif
}

void
machine_timer_stop(void)
{
#if defined (__AVR_ATmega128__)
    TIMSK &= ~(1 << TOIE0); /* disable interrupt on overflow */
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
    TIMSK0 &= ~(1 << TOIE0); /* disable interrupt on overflow */
#else
#error unsupported hardware
#endif
}

void
machine_timer_tick_isr(void)
{
}

ISR(TIMER0_OVF_vect)
{
   application_timer_isr_pointer();
}
