/* User code: This file will not be overwritten by TASTE. */

#include "oscillate.h"
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
#define M_PI 3.1416
#endif

/*
* This is a test function to produce a motion pattern for the CAN device used in ESROCOS
* Starting at 0 deg, this function will set the velocity to reach 135 deg.
* If 135 deg have been reached the function will try to reach 45 deg.
* Then it will indefintely cycle between 45 and 135 deg.
*/

enum State { REACH45P, REACH45N };
static enum State state = REACH45P;
static asn1SccBase_commands_Joints cmd;

uint64_t getTimeInMicroseconds()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
}

void oscillate_startup()
{
    printf("[oscillate_startup] startup\n");
}

void oscillate_PI_status(const asn1SccBase_commands_Joints *IN_sample)
{
    float position_rad = IN_sample->elements.arr[0].position;
    float position_deg = position_rad * 180.f / M_PI;
    float position_ref = (state == REACH45P) ? 135.f : 45.f;
    float error = (position_ref - position_deg);
    float velocity_ref_deg = fabsf(error) > 1.f ? ((error > 0.f) ? 1.f : ((error < 0.f) ? -1.f : 0.f)) : error;
    //printf("ref: %f cur: %f err: %f vel: %f\n", position_ref, position_deg, error, velocity_ref_deg);

    cmd.time.microseconds = getTimeInMicroseconds();
    cmd.names.nCount = 1;
    cmd.names.arr[0].nCount = snprintf((char*)cmd.names.arr[0].arr, 200, "JOINT0");
    cmd.elements.nCount = 1;
    cmd.elements.arr[0].speed = velocity_ref_deg * M_PI / 180.f;
    oscillate_RI_commands(&cmd);
    
    if (fabsf(error) < 2.f)
    {
	switch (state)
	{
	    case REACH45P:
		state = REACH45N;
		break;
	    case REACH45N:
		state = REACH45P;
		break;
	    default:
		break;
	}
    }
}

