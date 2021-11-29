#include "fileblocks.h"

#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    void grpWriteFileBlock(int ih, uint32_t fbn, void *buf)
    {
        soProbe(332, "%s(%d, %u, %p)\n", __FUNCTION__, ih, fbn, buf);
    
        /* replace the following line with your code */

        //ih-inode handler(ih is a valid handler of an inode in-use)
        //fbn-file block number 
        uint32_t block= soGetFileBlock(ih,fbn);//Data is written into a specific file block of an in-use inode

        if(block == BlockNullReference){ //If the referred block has not been allocated yet(has BlockNullReference)
        
            block= soAllocFileBlock(ih,fbn);//it should be allocated now so that the data can be stored as its contents;
        }
        //buf-pointer to the buffer containing data to be written
        soWriteDataBlock(block,buf);
        
        
        //binWriteFileBlock(ih, fbn, buf);
    }
};

