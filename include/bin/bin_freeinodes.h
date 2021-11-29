/**
 * \file
 * \brief Binary version of functions to manage the list of free inodes 
 *      and the list of free blocks
 * 
 *  \author Artur Pereira 2008-2009, 2016-2020
 *  \author Miguel Oliveira e Silva 2009, 2017
 *  \author António Rui Borges - 2010-2015
 *
 */

/*
 * \defgroup freeinodes freeinodes
 *
 * @{
 *
 *  \remarks In case an error occurs, every function throws an SOException
 */

#ifndef __SOFS20_FREEINODES_BIN__
#define __SOFS20_FREEINODES_BIN__

#include <inttypes.h>

namespace sofs20
{
    uint16_t binAllocInode(uint32_t mode);

    void binFreeInode(uint16_t in);
};

#endif /* __SOFS20_FREEINODES_BIN__ */
