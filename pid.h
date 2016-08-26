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
 *	pid.h
 *
 *	Created on: 27 aug 2016
 *      Author: Arvid Brodin
 */

#ifndef PID_H_
#define PID_H_

#include "sensor.h"

struct pid;
typedef struct pid	pid_t;

pid_t *pid_create(sensor_t *sensor);
void pid_destroy(pid_t *pid);

void pid_set_kp(pid_t *pid, float kp);
void pid_set_ki(pid_t *pid, float ki);
void pid_set_kd(pid_t *pid, float kd);
void pid_set_setpoint(pid_t *pid, float setpoint);
void pid_run(pid_t *pid);
float pid_get_output(pid_t *pid);

#endif /* PID_H_ */
