#include "fileblocks.h"

#include "freedatablocks.h"
#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <inttypes.h>
#include <errno.h>
#include <assert.h>

namespace sofs20
{
    /* free all blocks between positions ffbn and RPB - 1
     * existing in the block of references given by i1.
     * Return true if, after the operation, all references become BlockNullReference.
     * It assumes i1 is valid.
     */
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn);

    /* free all blocks between positions ffbn and RPB**2 - 1
     * existing in the block of indirect references given by i2.
     * Return true if, after the operation, all references become BlockNullReference.
     * It assumes i2 is valid.
     */
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn);

    /* ********************************************************* */

    void grpFreeFileBlocks(int ih, uint32_t ffbn)
    {
        soProbe(303, "%s(%d, %u)\n", __FUNCTION__, ih, ffbn);

        /* replace the following line with your code */
        //binFreeFileBlocks(ih, ffbn);
        SOInode *inode = soGetInodePointer(ih); // pointer to inode

        // frees all first fileblocks (d)
        for(; ffbn<N_DIRECT; ffbn++){
            if(inode->d[ffbn] != BlockNullReference){
                soFreeDataBlock(inode->d[ffbn]);
                inode->d[ffbn] = BlockNullReference;
                inode->blkcnt--;
            }
        }

        // update ffbn number
        ffbn -= N_DIRECT;

        // frees blocks at i1
        bool empty;
        uint32_t i;
        for(i=0; i<N_INDIRECT; i++){
            if(inode->i1[i] != BlockNullReference){
                empty = grpFreeIndirectFileBlocks(inode, inode->i1[i], ffbn);
                if (empty){
                    soFreeDataBlock(inode->i1[i]);
                    inode->i1[i] = BlockNullReference;
                    inode->blkcnt--;
                }
            }
            if (ffbn > RPB){
                ffbn -= RPB;
            }else{
                ffbn = 0;
            }
        }   

        // free blocks at i2
        for(i=0; i<N_DOUBLE_INDIRECT; i++){
            if(inode->i2[i] != BlockNullReference){
                empty = grpFreeDoubleIndirectFileBlocks(inode, inode->i2[i], ffbn);
                if(empty){
                    soFreeDataBlock(inode->i2[i]);
                    inode->i2[i] = BlockNullReference;
                    inode->blkcnt--;
                }
            }
            if (ffbn > RPB*RPB){
                ffbn -= RPB*RPB;
            }else{
                ffbn = 0;
            }
        }
    }

    /* ********************************************************* */

    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        /* replace the following line with your code */
        //throw SOException(ENOSYS, __FUNCTION__);

        // array with fileblock references 
        uint32_t ref[RPB];
        //reads block references
        soReadDataBlock(i1, ref);

        uint32_t reverse = ffbn+1;
        for(; ffbn<RPB; ffbn++){
            if(ref[ffbn] != BlockNullReference){
                soFreeDataBlock(ref[ffbn]);
                ref[ffbn] = BlockNullReference;
                ip->blkcnt--;
            }
        }

        soWriteDataBlock(i1, ref);

        for(; reverse>0; reverse--){
            if(ref[reverse-1] != BlockNullReference){
                return false;
            }
        }
        return true;
    }

    /* ********************************************************* */

    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i2, ffbn);

        /* replace the following line with your code */
        //throw SOException(ENOSYS, __FUNCTION__); 
        uint32_t ref[RPB];
        soReadDataBlock(i2, ref);

        uint32_t reverse = (ffbn/RPB)+1;
        bool empty;
        for(uint32_t i=0; i<RPB; i++) {
            if(ref[i] != BlockNullReference and ffbn<RPB) {
                empty = grpFreeIndirectFileBlocks(ip, ref[i], ffbn);
                // libertar i2[i] caso necessário
                if(empty) {
                    soFreeDataBlock(ref[i]);
                    ref[i] = BlockNullReference;
                    ip->blkcnt--;
                }
            }
            if (ffbn > RPB) {
                ffbn -= RPB;
            } else {
                ffbn = 0;
            }
        }

        soWriteDataBlock(i2, ref);

        for(; reverse>0; reverse--) {
            if(ref[reverse-1] != BlockNullReference) {
                return false;
            }
        }       
        return true;
    }
    /* ********************************************************* */
};

