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
#include <avr/io.h>

; extern void context_switch_avr(context_t to, context_t *from);
; extern void context_switch_avr(void *to, void **from);
.global context_switch_avr

; extern void context_switch_first_avr(context_t to);
; extern void context_switch_first_avr(void *to);
.global context_switch_first_avr

context_switch_avr:
    push    r0
    push    r1
    push    r2
    push    r3
    push    r4
    push    r5
    push    r6
    push    r7
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
    push    r16
    push    r17
    push    r28
    push    r29

    in      r18,SPL-__SFR_OFFSET
    in      r19,SPH-__SFR_OFFSET
    movw    r26,r22
    st      X+,r18
    st      X,r19

context_switch_first_avr:
    in      r20,SREG-__SFR_OFFSET
    cli

    out     SPL-__SFR_OFFSET,r24
    out     SPH-__SFR_OFFSET,r25

    out     SREG-__SFR_OFFSET,r20

    pop     r29
    pop     r28
    pop     r17
    pop     r16
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     r7
    pop     r6
    pop     r5
    pop     r4
    pop     r3
    pop     r2
    pop     r1
    pop     r0

    ret
