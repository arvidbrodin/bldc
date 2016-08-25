
#ifndef HALL_H_
#define HALL_H_

#include <stdint.h>

#define COMM_STEPS			6

void hall_init(int8_t *table);
void hall_set_table(int8_t *table);

int hall_read_phase(void);
int hall_comm_step_rev(int comm_step);

void hall_reset_detect_table(void);
void hall_sample(int comm_step);
int hall_get_detect_result(int8_t *table);

#endif /* HALL_H_ */
