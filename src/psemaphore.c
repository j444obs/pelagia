/* psemaphore.c - Time system related
*
* Copyright(C) 2019 - 2020, sun shuo <sun.shuo@surparallel.org>
* All rights reserved.
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or(at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.If not, see < https://www.gnu.org/licenses/>.
*/
#include "plateform.h"
#include "psemaphore.h"

//https://stackoverflow.com/questions/641126/posix-semaphores-on-mac-os-x-sem-timedwait-alternative

#ifdef __APPLE__
int sem_init(sem_t *psem, int flags, unsigned count) {

	NOTUSED(flags);
	int result;

	result = pthread_mutex_init(&psem->count_lock, NULL);
	if (result) {
		return result;
	}
	result = pthread_cond_init(&psem->count_bump, NULL);
	if (result) {
		return result;
	}
	psem->count = count;
	return 0;
}

int sem_destroy(sem_t *psem) {

	pthread_mutex_destroy(&psem->count_lock);
	pthread_cond_destroy(&psem->count_bump);
	return 0;
}

int sem_post(sem_t *psem) {
	int result, xresult;
	if (!psem) {
		return -1;
	}
	result = pthread_mutex_lock(&psem->count_lock);
	if (result){
		return result;
	}

	psem->count = psem->count + 1;
	xresult = pthread_cond_signal(&psem->count_bump);

	result = pthread_mutex_unlock(&psem->count_lock);
	if (result) {
		return result;
	}
	if (xresult) {
		return -1;
	}
	return 0;
}

int sem_trywait(sem_t *psem) {
	int result, xresult;

	if (!psem) {
		return -1;
	}

	result = pthread_mutex_lock(&psem->count_lock);
	if (result) {
		return result;
	}
	xresult = 0;

	if (psem->count > 0) {
		psem->count--;
	} else {
		xresult = -1;
	}
	result = pthread_mutex_unlock(&psem->count_lock);
	if (result) {
		return result;
	}
	if (xresult) {
		return -1;
	}
	return 0;
}

int sem_wait(sem_t *psem) {

	int result, xresult;

	if (!psem) {
		return -1;
	}

	result = pthread_mutex_lock(&psem->count_lock);
	if (result) {
		return result;
	}
	xresult = 0;

	if (psem->count == 0) {
		xresult = pthread_cond_wait(&psem->count_bump, &psem->count_lock);
	}
	if (!xresult) {
		if (psem->count > 0) {
			psem->count--;
		}
	}
	result = pthread_mutex_unlock(&psem->count_lock);
	if (result) {
		return result;
	}
	if (xresult) {
		return -1;
	}
	return 0;
}

int sem_timedwait(sem_t *psem, const struct timespec *abstim) {

	int result, xresult;
	if (!psem) {
		return -1;
	}

	result = pthread_mutex_lock(&psem->count_lock);
	if (result) {
		return result;
	}
	xresult = 0;

	if (psem->count == 0) {
		xresult = pthread_cond_timedwait(&psem->count_bump, &psem->count_lock, abstim);
	}
	if (!xresult) {
		if (psem->count > 0) {
			psem->count--;
		}
	}
	result = pthread_mutex_unlock(&psem->count_lock);
	if (result) {
		return result;
	}
	if (xresult) {
		return -1;
	}
	return 0;
}
#endif