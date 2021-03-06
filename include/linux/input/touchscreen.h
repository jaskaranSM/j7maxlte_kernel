/*
 * Copyright (c) 2014 Sebastian Reichel <sre@kernel.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#ifndef _TOUCHSCREEN_H
#define _TOUCHSCREEN_H

struct input_dev;

void touchscreen_parse_properties(struct input_dev *dev, bool multitouch);
/*add for touchscreen compatible by wxj in 2017.3.21*/
extern int touchscreen_load_status_check(void);
extern void touchscreen_load_status_set(void);
extern void touchscreen_load_status_clear(void);
/*end*/
#endif
