/*
 *	Copyright 2016 Arvid Brodin	arvidb@kth.se
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *	pid.c
 *
 *	Created on: 27 aug 2016
 *      Author: Arvid Brodin
 */

#include "pid.h"
#include "systime.h"
#include "utils.h"
#include <stdlib.h>

struct pid {
	float setpoint;
	float kp;
	float ki;
	float kd;
	float error_prev;
	float error_sum;
	float max_windup;
	sensor_t *sensor;
	systime_t timestamp_prev;
	float output_min;
	float output_max;
	float output;
};

pid_t *pid_create(sensor_t *sensor) {
	pid_t *pid = calloc(1, sizeof(*pid));
	if (pid == NULL) {
		fault();
	}
	pid->sensor = sensor;
	pid->output = 0.0;
	return pid;
}

void pid_destroy(pid_t *pid) {
	free(pid);
}

void pid_set_kp(pid_t *pid, float kp) {
	if (pid == NULL) {
		fault();
	}
	pid->kp = kp;
}

void pid_set_ki(pid_t *pid, float ki) {
	if (pid == NULL) {
		fault();
	}
	// Scale error_sum so we don't get a "bump" in the output
	pid->error_sum *= pid->ki;
	pid->ki = ki;
	pid->error_sum /= pid->ki;
}

void pid_set_kd(pid_t *pid, float kd) {
	if (pid == NULL) {
		fault();
	}
	pid->kd = kd;
}

void pid_set_setpoint(pid_t *pid, float setpoint) {
	if (pid == NULL) {
		fault();
	}
	pid->setpoint = setpoint;
}

void pid_run(pid_t *pid) {
	if (pid == NULL) {
		fault();
	}

	systime_t timestamp;
	const float feedback = sensor_get_value(pid->sensor, &timestamp, NULL);
	if (pid->timestamp_prev == 0) {
		// Initialise timestamp before first run
		pid->timestamp_prev = timestamp;
		return;
	}
	const float error = pid->setpoint - feedback;
	const float dt = systime_diff_f(pid->timestamp_prev, timestamp);
	pid->timestamp_prev = timestamp;

	pid->error_sum += error * dt;
	utils_truncate_number(&pid->error_sum, -pid->max_windup, pid->max_windup);

	float p_term = error * pid->kp;
	float i_term = pid->error_sum * pid->ki;
	float d_term = (error - pid->error_prev) /dt * pid->kd;

	pid->error_prev = error;

	pid->output = p_term + i_term + d_term;
	utils_truncate_number(&pid->output, pid->output_min, pid->output_max);
}

float pid_get_output(pid_t *pid) {
	if (pid == NULL) {
		fault();
	}
	return pid->output;
}
