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
 *	pid_cascade.h
 *
 *	Created on: 27 aug 2016
 *      Author: Arvid Brodin
 */

#ifndef PID_CASCADE_H_
#define PID_CASCADE_H_

#include "pid.h"

struct pid_cascade;
typedef struct pid_cascade	pid_cascade_t;

pid_cascade_t *nested_pid_create(void);
void pid_cascade_destroy(pid_cascade_t *cascade);
void pid_cascade_run(pid_cascade_t *cascade);
void pid_cascade_push(pid_cascade_t *cascade, pid_t *pid, int wait_factor);
pid_t *pid_cascade_pop(pid_cascade_t *cascade);
float pid_cascade_get_output(pid_cascade_t *cascade);

#endif /* PID_CASCADE_H_ */
