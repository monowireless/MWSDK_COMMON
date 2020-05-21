/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */


#include <jendefs.h>
#include "Version.h"

const uint8 au8ToCoNetUtilsVersionInfo[] = {
	'@', 'T', 'C', 'U', // MARKER STRING TO FIND EASILY
	0, VERSION_MAIN, VERSION_SUB, VERSION_VAR, // VERSION
	'U', 'C', 'T', '@' // MARKER STRING T FIND EASILY
};
