/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
 */

#include "freedatablocks.h"

#include <string.h>
#include <errno.h>
#include <iostream>

#include "core.h"
#include "devtools.h"
#include "daal.h"

namespace sofs20
{
    void grpReplenishRetrievalCache(void)
    {
        soProbe(443, "%s()\n", __FUNCTION__);

        /* replace the following line with your code */
        //binReplenishRetrievalCache();

        //First we get the reference to the superblock
        SOSuperblock *sb = soGetSuperblockPointer();
        
        //returns if the cache is not empty
        if(sb->retrieval_cache.idx != REF_CACHE_SIZE)
            return;

        //if the reference table still has free references left if it does we access it, otherwise we access
        // the insertion cache
        if(sb->reftable.count != 0){
            
            uint32_t *referenceBlockPointer = soGetReferenceBlockPointer(sb->reftable.blk_idx);


            //we pass the references from the ref table block to the retrieval cache
            //and null reference them
            
            //calculate end limiter of the data transfer
            uint32_t endLimiter = sb->reftable.ref_idx + REF_CACHE_SIZE-1;
            //if we surpasse the size of a block then we make the limit the end of the block
            if(endLimiter >= RPB){
                endLimiter = RPB-1; 
            }

            //set idx at the start again
            sb->retrieval_cache.idx = 0;
            for(uint32_t i = sb->reftable.ref_idx; i <= endLimiter; i++){
                //copy references
                sb->retrieval_cache.ref[sb->retrieval_cache.idx] = *(referenceBlockPointer+i);
                //null reference ref table positions
                *(referenceBlockPointer+i) = BlockNullReference;
                //update free references count
                sb->reftable.count--;
                sb->retrieval_cache.idx++;
            }

            //Debuging print
            /*for(uint32_t i = 0; i < RPB ; i++){
                std::cout << "reference " << i << " : " << *(referenceBlockPointer+i) << '\n';  
            }*/

            //we update the reference to the first available block to be the limiter + 1 and the retrieval cache back to the beginning           
            sb->reftable.ref_idx = endLimiter+1;
            sb->retrieval_cache.idx=0;

            //update reference table pointer, if it reaches the limit it goes back to the beginning
            //if we are at end of block we push the pointer to the next block on the reference table
            if(endLimiter == RPB-1){
                //we update the pointers yet again if a new block is arranged
                sb->reftable.blk_idx++;
                sb->reftable.ref_idx = 0;
                //if the new block is out of bounds we circle around to the beginning
                if(sb->reftable.blk_idx++ >= sb->rt_size-1)
                    sb->reftable.blk_idx = 0;              
            } 
            soSaveReferenceBlock();
        }
        else{
            //insertion cache
            //sets idx to end of retrieval cache
            sb->retrieval_cache.idx = 0;
            for(int i = sb->insertion_cache.idx; i > 0 ; i--){
                //gets insertion cache first free pos and passes it to retrieval cache
                sb->retrieval_cache.ref[sb->retrieval_cache.idx] = sb->insertion_cache.ref[i];
                //nulls it in insertion cache
                sb->insertion_cache.ref[i] = BlockNullReference;
                //moves idx of retrieval cache to the left;
                sb->retrieval_cache.idx++;   
            }
        }
        //save changes
        soSaveSuperblock();
    }
};

