#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "direntries.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

namespace sofs20
{
    uint16_t grpTraversePath(char *path)
    {
        soProbe(221, "%s(%s)\n", __FUNCTION__, path);

        char *base = basename(strdupa(path));
        char *dir = dirname(strdupa(path));

        if(strcmp(dir, "/") == 0)
        {
            int ih = soOpenInode(0);
            SOInode *inode = soGetInodePointer(ih);

            if(!soCheckInodeAccess(ih, X_OK)) throw SOException(EACCES, __FUNCTION__);
            if((inode->mode & S_IFDIR) != S_IFDIR) throw SOException(ENOTDIR, __FUNCTION__);
            return soGetDirentry(ih, base);
        }
        
        uint32_t in = soTraversePath(dir);
        int ih = soOpenInode(in);
        SOInode *inode = soGetInodePointer(ih);
        if(!soCheckInodeAccess(ih, X_OK)) throw SOException(EACCES, __FUNCTION__);
        if((inode->mode & S_IFDIR) != S_IFDIR) throw SOException(ENOTDIR, __FUNCTION__);
        return soGetDirentry(ih, base);
    }
};

