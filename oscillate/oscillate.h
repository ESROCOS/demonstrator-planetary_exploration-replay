/* This file was generated automatically: DO NOT MODIFY IT ! */

/* Declaration of the functions that have to be provided by the user */

#ifndef __USER_CODE_H_oscillate__
#define __USER_CODE_H_oscillate__

#include "C_ASN1_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

void oscillate_startup();

void oscillate_PI_status(const asn1SccBase_commands_Joints *);

extern void oscillate_RI_commands(const asn1SccBase_commands_Joints *);

#ifdef __cplusplus
}
#endif


#endif
