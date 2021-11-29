/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
 */

#include "freedatablocks.h"

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "core.h"
#include "devtools.h"
#include "daal.h"

namespace sofs20
{
    void grpFreeDataBlock(uint32_t bn)
    {
        soProbe(442, "%s(%u)\n", __FUNCTION__, bn);

        /* replace the following line with your code */
        // binFreeDataBlock(bn);

        SOSuperblock* superblock = soGetSuperblockPointer();

        if(bn>superblock->ntotal || bn<0)
        {
            throw SOException(EINVAL,__FUNCTION__);
        }

        if(superblock->insertion_cache.idx == REF_CACHE_SIZE)
        {
            soDepleteInsertionCache();
        }

        superblock->insertion_cache.ref[superblock->insertion_cache.idx] = bn;
        superblock->dbfree++;
        superblock->insertion_cache.idx++;

        soSaveSuperblock();
    }
};

