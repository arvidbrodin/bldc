
#include "hall.h"

// CHECK: "hal.h" is a dependency for hw_410.h and should really be included there
#include "hal.h"
#include "hw.h"

#include <string.h>

#define HALL_STATES			8

static int hall_to_phase_table[HALL_STATES];

// CHECK: Why is this declared volatile?
static volatile int hall_detect_table[HALL_STATES][COMM_STEPS];


/**
 * Initialize the hall sensor lookup table
 *
 * @param table
 * The commutations corresponding to the hall sensor states in the forward direction.
 * Range [1..6]
 */
void hall_set_table(int8_t *table) {
	for (int hall_state = 0; hall_state < HALL_STATES; hall_state++) {
		// table[] has values [1..6], convert to [0..5]
		int ind_now = table[hall_state];
		if (ind_now > -1) {
			ind_now--;
		}
		hall_to_phase_table[hall_state] = ind_now;
	}
}

/**
 * Reset the hall sensor detection table
 */
void hall_reset_detect_table(void) {
	memset(hall_detect_table, 0, sizeof(hall_detect_table));
}

void hall_init(int8_t *table) {
	hall_reset_detect_table();
	hall_set_table(table);
}


/**
 * Get the current detected hall sensor table
 *
 * @param table
 * Pointer to a table where the result should be stored (range [1..6]).
 *
 * @return
 * 0: OK
 * -1: Invalid hall sensor output
 */
int hall_get_detect_result(int8_t *table) {
	for (int hall_state = 0; hall_state < HALL_STATES; hall_state++) {
		int samples = 0;
		int res = -1;
		for (int comm_step = 0; comm_step < COMM_STEPS; comm_step++) {
			if (hall_detect_table[hall_state][comm_step] > samples) {
				samples = hall_detect_table[hall_state][comm_step];
				if (samples > 15) {
					res = comm_step;
				}
			}
			table[hall_state] = res;
		}
	}

	int invalid_samp_num = 0;
	int nums[COMM_STEPS] = {0, 0, 0, 0, 0, 0};
	int tot_nums = 0;
	for (int hall_state = 0; hall_state < HALL_STATES; hall_state++) {
		if (table[hall_state] == -1) {
			invalid_samp_num++;
		} else {
			if (!nums[table[hall_state]]) {
				nums[table[hall_state]] = 1;
				tot_nums++;
			}
		}
	}

	for (int hall_state = 0; hall_state < HALL_STATES; hall_state++) {
		// Preserve range [1..6] for communication with bldc-tool etc.
		if (table[hall_state] > -1)
			table[hall_state]++;
	}

	if (invalid_samp_num == 2 && tot_nums == 6) {
		return 0;
	} else {
		return -1;
	}
}

static int read_hall(void) {
	return READ_HALL1() | (READ_HALL2() << 1) | (READ_HALL3() << 2);
}

void hall_sample(int comm_step) {
	hall_detect_table[read_hall()][comm_step]++;
}

/**
 * Read the current phase of the motor using hall effect sensors
 * @return
 * The phase read.
 */
int hall_read_phase(void) {
	return hall_to_phase_table[read_hall()];
}

int hall_comm_step_rev(int comm_step) {
	// This is equivalent to the earlier transformation using the
	// lookup table fwd_to_rev[COMM_STEPS] = {0,5,4,3,2,1};
	return (COMM_STEPS - comm_step) % COMM_STEPS;
}
