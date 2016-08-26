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
 *	systime.h
 *
 *	Created on: 27 aug 2016
 *      Author: Arvid Brodin
 */

#ifndef SYSTIME_H_
#define SYSTIME_H_

#include <stdint.h>

typedef int64_t	systime_t;

static inline float systime_diff_f(systime_t first, systime_t second) {
	systime_t diff = second - first;
	return (float) diff / 1e9;
}

#endif /* SYSTIME_H_ */
