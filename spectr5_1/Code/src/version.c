/*!****************************************************************************
* @file     version
* @author   d_el - Storozhenko Roman
* @date     26.12.2018 23:20:40
* @major    1
* @minor    0
* @revision 0
* @build    97
*/

#include "version.h"

const uint32_t fwinfo_major    = 1;
const uint32_t fwinfo_minor    = 0;
const uint32_t fwinfo_revision = 0;
const uint32_t fwinfo_build    = 97;
const char fwinfo_version[] = {"1.0.0.97"};
const char fwinfo_date[]    = {"26.12.2018 23:20:40"};

#include "stdint.h"

extern const uint32_t fwinfo_major;
extern const uint32_t fwinfo_minor;
extern const uint32_t fwinfo_revision;
extern const uint32_t fwinfo_build;
extern const char fwinfo_version[];
extern const char fwinfo_date[];

/***************** (C) COPYRIGHT ************** END OF FILE ******** d_el ****/
