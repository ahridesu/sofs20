#include "fileblocks.h"

#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    void grpReadFileBlock(int ih, uint32_t fbn, void *buf)
    {
        soProbe(331, "%s(%d, %u, %p)\n", __FUNCTION__, ih, fbn, buf);

        /* replace the following line with your code */
        // binReadFileBlock(ih, fbn, buf);

        uint32_t dbn= soGetFileBlock(ih,fbn);

        if(dbn == BlockNullReference)
        {
            char temp[BlockSize] = {'\0'};
            buf = &temp;
        }

        soReadDataBlock(dbn, buf);
    }
};

