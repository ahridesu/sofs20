/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
 */

#include "freedatablocks.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;

namespace sofs20
{
    /* only fill the current block to its end */
    void grpDepleteInsertionCache(void)
    {
        soProbe(444, "%s()\n", __FUNCTION__);
        //binDepleteInsertionCache();

        /* Open Superblock and get pointer to Superblock */
        soOpenSuperblock();
        SOSuperblock* sb = soGetSuperblockPointer();

        uint32_t ref[RPB];
        uint32_t nfree = RPB - sb->insertion_cache.idx;
        soReadDataBlock(sb->insertion_cache.idx, ref);

        /* Nothing is done if the cache is not full. */
        if(sb->insertion_cache.idx == REF_CACHE_SIZE) 
        {
            if(sb->reftable.count == RPB)
            {
                uint32_t newBlock = soAllocDataBlock();
                ref[0] = newBlock;
                soWriteDataBlock(sb->reftable.blk_idx, ref);
                uint32_t newRef[RPB];
                
                for(uint32_t i = 0; i < RPB; i++)
                {
                    newRef[i] = BlockNullReference;
                }

                soWriteDataBlock(newBlock, newRef);
                sb->reftable.blk_idx = newBlock;
                sb->reftable.ref_idx = 1;
            }

            nfree = RPB - sb->insertion_cache.idx;
            
            if(nfree >= REF_CACHE_SIZE)
            {
                soReadDataBlock(sb->reftable.blk_idx, ref);
                sb->insertion_cache.idx = 0;
                for(uint32_t i = 0; i < REF_CACHE_SIZE; i++)
                {
                    ref[sb->reftable.ref_idx+sb->reftable.count] = sb->insertion_cache.ref[i];
                    sb->reftable.ref_idx++;
                    sb->insertion_cache.ref[i] = BlockNullReference;
                }
                soWriteDataBlock(sb->reftable.blk_idx, ref);
            }
            else
            {
                uint32_t idx = nfree + 1;
                for(uint32_t i = 0; i < nfree; i++)
                {
                    ref[sb->reftable.ref_idx+sb->reftable.count] = sb->insertion_cache.ref[i];
                    sb->reftable.ref_idx++;
                    sb->insertion_cache.ref[i] = BlockNullReference;
                }
                soWriteDataBlock(sb->reftable.blk_idx, ref);

                for(uint32_t k = 0; k < REF_CACHE_SIZE; k++)
                {
                    if(k < REF_CACHE_SIZE - nfree)
                    {
                        sb->insertion_cache.ref[k] = idx++;
                    }
                    else
                    {
                        sb->insertion_cache.ref[k] = BlockNullReference;
                    }
                }
                sb->reftable.ref_idx = REF_CACHE_SIZE - nfree;
            }
        }
        soSaveSuperblock();

    }
};

