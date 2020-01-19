#include "tptime.h"

#include <msp430.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "chrt.h"
#include "mspbase.h"

/* Expiration time struct, usable in linked list. */
struct exp_time {
  void* tag;
  uint32_t time;
  struct exp_time* next;
};

/* Head of linked list of expiration times. */
__nv struct exp_time* exp_time_head;

/* Absolute system time, updated at each reboot using CHRT. */
__nv uint32_t sys_time;

__nv static uint8_t first_boot;

static void start_sys_timer(void) {
  /* Configure SMCLK at 0.25 MHz. */
  msp_clock_set_mclk(CLK_8_MHZ, SMCLK_0_25_MHZ);

  /* Configure timer TA0: */
  /* - sourced by SMCLK (0.25 MHz) */
  /* - clock divided by 8 * 8 = 64 */
  /* => 1 tick is 0.256 ms, counting at most up to 16766 ms */
  /* TODO: implement overflow handling with interrupt */
  TA0CTL = TASSEL__SMCLK | ID__8;
  TA0EX0 = TAIDEX__8;

  /* Start timer TA0. */
  TA0CTL |= TACLR;
  TA0CTL |= MC__CONTINUOUS;
}

static void insert_exp_time(struct exp_time* new_exp_time) {
  struct exp_time* current = exp_time_head;
  struct exp_time* previous = NULL;

  /* Traverse linked list, which is sorted by (ascending) expiration time, to
   * insert new_exp_time. */
  while (current != NULL) {
    if (current->time >= new_exp_time->time) {
      if (previous == NULL) {
        exp_time_head = new_exp_time;
      } else {
        previous->next = new_exp_time;
      }
      new_exp_time->next = current;
      return;
    }
    previous = current;
    current = current->next;
  }

  /* Insert new_exp_time at the end of the linked list. */
  previous->next = new_exp_time;
  new_exp_time->next = current;

  /* Assign linked list head in case it points to NULL. */
  if (exp_time_head == NULL) {
    exp_time_head = new_exp_time;
  }
}

void tptime_init(void) {
  /* Initialize CHRT. */
    chrt_init(TPTIME_CHRT_START_TIER, TPTIME_CHRT_MARGIN_UP,
              TPTIME_CHRT_MARGIN_DOWN, TPTIME_CHRT_FIX_TIERS);

  /* Increment absolute system time and recharge CHRT. */
  uint16_t resolution, time;
  chrt_get_time(&resolution, &time, true);
  chrt_charge();

  sys_time += (uint32_t)time * resolution;

  /* Initialize non-volatile variables on first boot. */
  if (first_boot) {
    sys_time = 0;
    exp_time_head = NULL;
    first_boot = 0;
  }

  /* Start digital timer to keep time since last reboot. */
  start_sys_timer();
}

uint32_t tptime_get_timestamp(void) {
  /* Current time is the sum of absolute system time until the last reboot,
   * and time since last reboot.  Absolute time is maintained with the CHRT,
   * whilst time since last reboot is kept with a digital timer.  One digital
   * timer tick is 0.256 ms, thus divide ticks by 4 to obtain milliseconds
   * (approximately). */
  uint32_t timestamp = sys_time + TA0R / 4;
  return timestamp;
}

int8_t tptime_set_expiration(void* tag, uint32_t expiration) {
  /* Search list of expiration times linearly, from earliest to latest. */
  struct exp_time* e = exp_time_head;
  while (e != NULL) {
    /* If the given tag is found in list, set new expiration time for the
     * corresponding object. */
    if (e->tag == tag) {
      e->time = expiration + tptime_get_timestamp();
      return TPTIME_OK;
    }
    e = e->next;
  }

  /* Allocate new expiration time. */
  struct exp_time* new_exp_time = malloc(sizeof(struct exp_time));
  if (new_exp_time == NULL) {
    return TPTIME_MEM_ERROR;
  }

  /* Insert new expiration time in sorted linked list. */
  new_exp_time->tag = tag;
  new_exp_time->time = expiration + tptime_get_timestamp();
  insert_exp_time(new_exp_time);
  return TPTIME_OK;
}

int8_t tptime_clear_expiration(void* tag) {
  /* Traverse linked list, which is sorted by (ascending) expiration time, to
   * remove tag. */
  struct exp_time* current = exp_time_head;
  struct exp_time* previous = NULL;
  while (current != NULL) {
    /* If the given tag is found in list, clear expiration time for the
     * corresponding object. */
    if (current->tag == tag) {
      if (current == exp_time_head) {
        exp_time_head = current->next;
      } else {
        previous->next = current->next;
      }
      free(current);
      return TPTIME_OK;
    }
    previous = current;
    current = current->next;
  }

  /* Object not found. */
  return TPTIME_OBJ_NOT_FOUND;
}

int8_t tptime_has_expired(void* tag) {
  /* Search list of expiration times linearly, from earliest to latest. */
  struct exp_time* e = exp_time_head;
  while (e != NULL) {
    if (e->tag == tag) {
      /* Object found, return EXPIRED or VALID. */
      if (e->time > tptime_get_timestamp()) {
        return TPTIME_OBJ_VALID;
      } else {
        return TPTIME_OBJ_EXPIRED;
      }
    }
    e = e->next;
  }

  /* Object not found. */
  return TPTIME_OBJ_NOT_FOUND;
}
