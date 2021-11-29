#include "fileblocks.h"

#include "freedatablocks.h"
#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <errno.h>

#include <iostream>

namespace sofs20
{

//#if false
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn);
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn);
//#endif

    /* ********************************************************* */

    uint32_t grpAllocFileBlock(int ih, uint32_t fbn)
    {
        soProbe(302, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /* replace the following two lines with your code */
        //return binAllocFileBlock(ih, fbn);
        SOInode *inode = soGetInodePointer(ih);

        uint32_t allocblock;

        //means its a direct allocation
        if(fbn<N_DIRECT){
            allocblock = soAllocDataBlock();
            inode->d[fbn] = allocblock;
            inode->blkcnt++;
        }
        else if(fbn < (N_DIRECT + N_INDIRECT * RPB)){
            allocblock = grpAllocIndirectFileBlock(inode,fbn-N_DIRECT);
        }
        else if(fbn < N_DIRECT + N_INDIRECT*RPB*(N_DOUBLE_INDIRECT*RPB)){
            allocblock = grpAllocDoubleIndirectFileBlock(inode, fbn - (N_DIRECT + N_INDIRECT*RPB));
        }
        else{
            throw SOException(EINVAL,__FUNCTION__);
            return 0;
        }

        soSaveInode(ih);

        return allocblock;
    }

    /* ********************************************************* */

//#if false
    /*
    */
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* replace the following two lines with your code */
        //throw SOException(ENOSYS, __FUNCTION__); 
        //return 0;

        //i[*] calculating position on that array
        uint32_t ipos = afbn/RPB;
        //block inside that ipos
        uint32_t iblock;
        //array that we will use to substitute
        uint32_t ref[RPB];

        //block doesnt exist there yet
        //BlockNullReference
        if(ip->i1[ipos] == BlockNullReference){
            iblock = soAllocDataBlock();

            for(uint32_t i=0; i<RPB ; i++){
                ref[i] = BlockNullReference;
            }

            ip->i1[ipos] = iblock;
            ip->blkcnt++;
        }
        else{
            iblock = ip->i1[ipos];
            soReadDataBlock(iblock,ref);
        }

        uint32_t allocblock = soAllocDataBlock();
        ref[afbn%RPB] = allocblock;

        ip->blkcnt++;

        soWriteDataBlock(iblock,ref);
        return allocblock;

    }
//#endif

    /* ********************************************************* */

//#if false
    /*
    */
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* replace the following two lines with your code */
        //throw SOException(ENOSYS, __FUNCTION__); 
        //return 0;
        uint32_t ipos = afbn/(RPB*RPB);
        uint32_t diblock;
        uint32_t iref[RPB];

        if(ip->i2[ipos] == BlockNullReference){
            diblock = soAllocDataBlock();
            ip->i2[ipos] = diblock;
            ip->blkcnt++;
            for(uint32_t i=0; i<RPB ; i++){
                iref[i] = BlockNullReference;
            }

        }else{
            diblock=ip->i2[ipos];
            soReadDataBlock(diblock,iref);
        }

        u_int32_t allocblock = grpAllocIndirectFileBlock(ip,afbn-ipos*(RPB*RPB));

        soWriteDataBlock(diblock,iref);

        return allocblock;
    }
//#endif
};

