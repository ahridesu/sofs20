/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2009-2020
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
    uint32_t grpAllocDataBlock()
    {
        soProbe(441, "%s()\n", __FUNCTION__);

        /* replace the following line with your code */
        //Get a pointer to the superblock and returns a pointer to the superblock in memory
        SOSuperblock *superblock= soGetSuperblockPointer();//A block named superblock is used for general metadata

        //If the retrieval chache is empty, the replenish function must be called first
        if(superblock->retrieval_cache.idx == REF_CACHE_SIZE){
            soReplenishRetrievalCache();//When calling a function of any layer, the version with prefix so should be used
            
        }
        //The first reference in the retrieval cache must be retrieved and returned
        uint32_t nblocks= superblock->retrieval_cache.ref[superblock->retrieval_cache.idx];
        superblock->retrieval_cache.ref[superblock->retrieval_cache.idx]= BlockNullReference;//idx is the index of the first free/ocupied cell
       
        //The data block meta data fields in the superblock must be updated
        superblock->dbfree--;//Decrement the number of free data blocks(because one of them was occupied)
        superblock->retrieval_cache.idx++;//Increment the retrieval cache of references to free data blocks(como houve uma que foi recuperada, incrementa 1)
        
        //Exceptions
        //ENOSPC if there are no free datablocks
        if(superblock->dbfree == 0)//if the number of free data blocks is 0
        {
            throw SOException(ENOSPC, __FUNCTION__);
        }

        soSaveSuperblock();
        return nblocks;//Return the number(reference) of the data block allocated

        //return binAllocDataBlock();
    }
};


