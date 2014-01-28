#include "syncmgr.h"

/**
 * Check if a filename (absolute) is already opened
 **/
bool FileSyncMgr::IsOpenedFile(const char *name)
{
    return fileNames.find(std::string(name)) != fileNames.end();
}

/**
 * Mark a filename as opened
 **/
void FileSyncMgr::NewOpenedFile(const char *name)
{
    if (IsOpenedFile(name))
        fileNames[std::string(name)]++;
    else
        fileNames[std::string(name)] = 1;
}

/**
 * Mark a filename as closed
 **/
void FileSyncMgr::DeleteOpenedFile(const char *name)
{
    ASSERT(IsOpenedFile(name));

    fileNames[std::string(name)]--;

    if (fileNames[std::string(name)] == 0)
        fileNames.erase(std::string(name));
}

/**
 * Get sync structure for opening file
 **/
FileHdrSync *FileSyncMgr::GetFileSyncForFile(int sector)
{
    std::map<int, FileHdrSync*>::iterator itr = fileSync.find(sector);

    // If already inside map, inc reference and return structure
    if (itr != fileSync.end())
    {
        itr->second->reference++;
        return itr->second;
    }
    else
    {
        fileSync[sector] = new FileHdrSync;
        fileSync[sector]->reference = 1;
        fileSync[sector]->lock = new Lock("file sync lock");
    }

    return fileSync[sector];
}

/**
 * Remove sync reference when closing file
 * If last reference, delete structure
 **/
void FileSyncMgr::DetachFileSyncForFile(int sector)
{
    std::map<int, FileHdrSync*>::iterator itr = fileSync.find(sector);

    // Make sure already existing
    ASSERT(itr != fileSync.end());

    itr->second->reference--;

    // If last, delete structure
    if (itr->second->reference == 0)
    {
        delete itr->second->lock;
        delete itr->second;
        fileSync.erase(itr);
    }
}
