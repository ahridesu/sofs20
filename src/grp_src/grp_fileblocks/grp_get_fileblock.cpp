#include "fileblocks.h"

#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <errno.h>

namespace sofs20
{
    /* ********************************************************* */

    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t fbn);
    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t fbn);

    /* ********************************************************* */

    uint32_t grpGetFileBlock(int ih, uint32_t fbn)
    {
        soProbe(301, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /* Get Inode pointer */
        SOInode *inode = soGetInodePointer(ih);
        uint32_t result;
        
        /* Check if fbn is valid. If not throw EINVAL exception */
        if(fbn < 0)
        {
            throw SOException(EINVAL, __FUNCTION__);
        }

        /* Check if fbn is direct, indirect or double indirect */
        if(fbn < N_DIRECT){
            result = inode->d[fbn];
        }
        else if(fbn < (N_DIRECT * RPB + N_DIRECT))
        {
            result = grpGetIndirectFileBlock(inode, fbn);
        }
        else
        {
            result = grpGetDoubleIndirectFileBlock(inode, fbn);
        }

        /* Save Inode and return block number */
        soSaveInode(ih);
        return result;
    }

    /* ********************************************************* */

    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* Gets the index inside the indirect zone */
        uint32_t i1_idx = afbn / RPB;
        /* Gets the index inside the referenced block */
        uint32_t i1_bn = afbn % RPB;
        uint32_t ref[RPB];

        /* If indirect block is Null return BlockNullReference */
        soReadDataBlock(ip->i1[i1_idx], ref);
        if(ip->i1[i1_idx] == BlockNullReference)
        {
            return BlockNullReference;
        }
        else
        {
            /* Read datablock to buffer */
        soReadDataBlock(ip->i1[i1_idx], ref);
            return ref[i1_bn];
        }

        throw SOException(EINVAL, __FUNCTION__);
    }

    /* ********************************************************* */

    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);


        uint32_t ref[RPB];
        /* Gets the index inside the double indirect zone */
        uint32_t p1 = afbn / (RPB*RPB);
         /* Gets the index inside the indirect zone */
        uint32_t p2 = afbn / (RPB-(p1*RPB));
        /* Gets the index inside the referenced block */
        uint32_t p3 = afbn % RPB;

         /* If double indirect block is Null return BlockNullReference */
        if(ip->i2[p1] == BlockNullReference)
        {
            return BlockNullReference;
        }
        else
        {
            soReadDataBlock(ip->i2[p1], ref);

            /* If indirect block is Null return BlockNullReference */
            if(ref[p2] == BlockNullReference)
            {
                return BlockNullReference;
            }
            else
            {
                soReadDataBlock(ip->i2[p2], ref);
                return ref[p3];
            }
        }

        throw SOException(EINVAL, __FUNCTION__);
    }
};

