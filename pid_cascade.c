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
 *	pid_cascade.c
 *
 *	Created on: 27 aug 2016
 *      Author: Arvid Brodin
 */

#include "pid_cascade.h"
#include <stdlib.h>

struct pid_item {
	pid_t *pid;
	int wait;
	struct pid_item *next;
};

struct pid_cascade {
	struct pid_item *pidi_first;
	int iteration;
	float output;
};


pid_cascade_t *pid_cascade_create(void) {
	struct pid_cascade *cascade = calloc(1, sizeof(*cascade));
	cascade->output = 0.0;
	return cascade;
}

void pid_cascade_destroy(pid_cascade_t *cascade) {
	if (cascade == NULL)
		fault();

	while (cascade->pidi_first) {
		pid_t *pid = pid_cascade_pop(cascade);
		pid_destroy(pid);
	}
	free(cascade);
}


void pid_cascade_run(pid_cascade_t *cascade) {
	if (cascade == NULL)
		fault();

	struct pid_item *pidi = cascade->pidi_first;
	if (pidi == NULL) {
		cascade->output = 0.0;
		return;
	}

	if (cascade->iteration >= pidi->wait) {
		cascade->iteration = 0;
	}

	while (pidi) {
		if (cascade->iteration % pidi->wait == 0) {
			pid_run(pidi->pid);
			if (pidi->next) {
				pid_set_setpoint(pidi->next->pid, pid_get_output(pidi->pid));
			} else {
				cascade->output = pid_get_output(pidi->pid);
			}
		}
		pidi = pidi->next;
	}

	cascade->iteration += 1;
}

void pid_cascade_push(pid_cascade_t *cascade, pid_t *pid, int wait_factor) {
	if (cascade == NULL)
		fault();

	struct pid_item *pidi = calloc(1, sizeof(*pidi));
	pidi->pid = pid;
	if (cascade->pidi_first) {
		pidi->wait = cascade->pidi_first->wait * wait_factor;
	} else {
		pidi->wait = wait_factor;
	}
	pidi->next = cascade->pidi_first;
	cascade->pidi_first = pidi;
}

pid_t *pid_cascade_pop(pid_cascade_t *cascade) {
	if (cascade == NULL)
		fault();

	struct pid_item *pidi = cascade->pidi_first;
	if (pidi == NULL) {
		return NULL;
	}
	cascade->pidi_first = pidi->next;

	pid_t *pid;
	pid = pidi->pid;
	free(pidi);

	return pid;
}

float pid_cascade_get_output(pid_cascade_t *cascade) {
	if (cascade == NULL)
		fault();

	return cascade->output;
}
