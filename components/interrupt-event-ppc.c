/*| public_headers |*/

/*| public_type_definitions |*/

/*| public_structure_definitions |*/

/*| public_object_like_macros |*/

/*| public_function_like_macros |*/

/*| public_extern_definitions |*/

/*| public_function_definitions |*/
{{#interrupt_events.length}}
void {{prefix_func}}interrupt_event_raise({{prefix_type}}InterruptEventId event);
{{/interrupt_events.length}}

/*| headers |*/
{{#interrupt_events.length}}
#include <stdint.h>
#include <stdbool.h>
{{/interrupt_events.length}}

/*| object_like_macros |*/

/*| type_definitions |*/

/*| structure_definitions |*/

/*| extern_definitions |*/

/*| function_definitions |*/
static void interrupt_event_process(void);
static inline bool interrupt_application_event_check(void);
static inline void interrupt_event_wait(void);

/*| state |*/
{{#interrupt_events.length}}
static volatile uint32_t interrupt_event;
{{/interrupt_events.length}}

/*| function_like_macros |*/

/*| functions |*/
static void
interrupt_event_process(void)
{
{{#interrupt_events.length}}
    uint32_t tmp = interrupt_event;
    while (tmp != 0)
    {
        {{prefix_type}}InterruptEventId i = __builtin_ffs(tmp) - 1;
        interrupt_event &= ~(1U << i);
        handle_interrupt_event(i);
        tmp &= ~(1U << i);
    }
{{/interrupt_events.length}}
}

static inline bool
interrupt_application_event_check(void)
{
{{#interrupt_events.length}}
    return interrupt_event != 0;
{{/interrupt_events.length}}
{{^interrupt_events.length}}
    return false;
{{/interrupt_events.length}}
}

static inline void
interrupt_event_wait(void)
{
    asm volatile("wrteei 0"); /* Clear MSR[EE] to disable noncritical interrupts */
    asm volatile("isync");
    if (!interrupt_event_check())
    {
        /* PowerPC e500 doesn't appear to implement the "wait" instruction, so we do things the e500 way.
         * The msync-mtmsr(WE)-isync sequence is explicitly recommended by the e500 Core Family Reference Manual. */
        asm volatile(
            "mfmsr %%r3\n"
            "oris %%r3,%%r3,0x4\n" /* Set 0x40000 = MSR[WE] to gate DOZE/NAP/SLEEP depending on how HID0 is set */
            "ori %%r3,%%r3,0x8000\n" /* Set 0x8000 = MSR[EE] to enable noncritical interrupts */
            "msync\n"
            "mtmsr %%r3\n"
            "isync\n"
            ::: "r3");
    }
    asm volatile("wrteei 1"); /* Set MSR[EE] to enable noncritical interrupts */
}

/*| public_functions |*/
{{#interrupt_events.length}}
void
{{prefix_func}}interrupt_event_raise({{prefix_type}}InterruptEventId interrupt_event_id)
{
    interrupt_event |= 1U << interrupt_event_id;
}
{{/interrupt_events.length}}