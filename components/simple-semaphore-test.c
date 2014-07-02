/*| schema |*/
<entry name="prefix" type="ident" optional="true" />
<entry name="semaphores" type="list" auto_index_field="idx">
    <entry name="semaphore" type="dict">
        <entry name="name" type="ident" />
    </entry>
</entry>
<entry name="tasks" type="list" auto_index_field="idx">
    <entry name="task" type="dict">
        <entry name="name" type="ident" />
    </entry>
</entry>

/*| public_headers |*/

/*| public_type_definitions |*/
typedef uint8_t {{prefix_type}}TaskId;

/*| public_structure_definitions |*/

/*| public_object_like_macros |*/
#define {{prefix_const}}TASK_ID_ZERO (({{prefix_type}}TaskId) UINT8_C(0))
#define {{prefix_const}}TASK_ID_MAX (({{prefix_type}}TaskId)UINT8_C({{tasks.length}} - 1))

/*| public_function_like_macros |*/

/*| public_extern_definitions |*/

/*| public_function_definitions |*/

/*| headers |*/
#include <stdio.h>
#include <stddef.h>
#include "rtos-simple-semaphore-test.h"

/*| object_like_macros |*/

/*| type_definitions |*/

/*| structure_definitions |*/

/*| extern_definitions |*/

/*| function_definitions |*/

/*| state |*/
static void (*block_ptr)(void);
static void (*unblock_ptr)({{prefix_type}}TaskId);
static {{prefix_type}}TaskId (*get_current_task_ptr)(void);

/*| function_like_macros |*/
#define preempt_enable()
#define preempt_disable()
#define api_assert(expression, error_id) do { } while(0)

/*| functions |*/
static void
_block(void) {{prefix_const}}REENTRANT
{
    if (block_ptr != NULL)
    {
        block_ptr();
    }
}

static void
_unblock({{prefix_type}}TaskId task_id)
{
    if (unblock_ptr != NULL)
    {
        unblock_ptr(task_id);
    }
}

static {{prefix_type}}TaskId
get_current_task(void)
{
    if (get_current_task_ptr != NULL)
    {
        return get_current_task_ptr();
    }

    return {{prefix_const}}TASK_ID_ZERO;
}

/*| public_functions |*/

struct semaphore * pub_semaphores = semaphores;
{{prefix_type}}TaskId * pub_waiters = waiters;

void pub_set_block_ptr(void (*fn)(void))
{
    block_ptr = fn;
}

void pub_set_unblock_ptr(void (*fn)({{prefix_type}}TaskId))
{
    unblock_ptr = fn;
}

void pub_set_get_current_task_ptr({{prefix_type}}TaskId (*y)(void))
{
    get_current_task_ptr = y;
}

void pub_sem_init(void)
{
    {{prefix_type}}SemId sem_id;
    sem_init();
    /* For testing purposes we also reset the value of all semaphores to zero */
    for (sem_id = SEM_ID_ZERO; sem_id <= SEM_ID_MAX; sem_id++)
    {
        semaphores[sem_id].value = SEM_VALUE_ZERO;
    }
    block_ptr = NULL;
    unblock_ptr = NULL;
    get_current_task_ptr = NULL;
}