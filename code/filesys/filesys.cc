// filesys.cc
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//     A file header, stored in a sector on disk
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//     A number of data blocks
//     An entry in the file system directory
//
//  The file system consists of several data structures:
//     A bitmap of free disk sectors (cf. bitmap.h)
//     A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them
//	on bootup.
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
//  Our implementation at this point has the following restrictions:
//
//     there is no synchronization for concurrent accesses
//     files have a fixed size, set when the file is created
//     files cannot be bigger than about 3KB in size
//     there is no hierarchical directory structure, and only a limited
//       number of files can be added to the system
//     there is no attempt to make the system robust to failures
//      (if Nachos exits in the middle of an operation that modifies
//      the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "disk.h"
#include "bitmap.h"
#include "directory.h"
#include "filehdr.h"
#include "filesys.h"
#include "system.h"
#include <libgen.h>
#include <list>
#include <string>

//----------------------------------------------------------------------
// FileSystem::FileSystem
//  Initialize the file system.  If format = TRUE, the disk has
//	nothing on it, and we need to initialize the disk to contain
//	an empty directory, and a bitmap of free sectors (with almost but
//	not all of the sectors marked as free).
//
//	If format = FALSE, we just have to open the files
//	representing the bitmap and the directory.
//
//	"format" -- should we initialize the disk?
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format)
{
    DEBUG('f', "Initializing the file system.\n");
    if (format) {
        BitMap *freeMap = new BitMap(NumSectors);
        Directory *directory = new Directory(NumDirEntries);
        FileHeader *mapHdr = new FileHeader;
        FileHeader *dirHdr = new FileHeader;

        DEBUG('f', "Formatting the file system.\n");

        // First, allocate space for FileHeaders for the directory and bitmap
        // (make sure no one else grabs these!)
        freeMap->Mark(FreeMapSector);
        freeMap->Mark(DirectorySector);

        // Second, allocate space for the data blocks containing the contents
        // of the directory and bitmap files.  There better be enough space!

        ASSERT(mapHdr->AskForSectors(freeMap, FreeMapFileSize));
        ASSERT(dirHdr->AskForSectors(freeMap, DirectoryFileSize));

        // Flush the bitmap and directory FileHeaders back to disk
        // We need to do this before we can "Open" the file, since open
        // reads the file header off of disk (and currently the disk has garbage
        // on it!).

        DEBUG('f', "Writing headers back to disk.\n");
        mapHdr->WriteBack(FreeMapSector);
        dirHdr->WriteBack(DirectorySector);

        // OK to open the bitmap and directory files now
        // The file system operations assume these two files are left open
        // while Nachos is running.

        freeMapFile = new OpenFile(FreeMapSector, NULL);
        directoryFile = new OpenFile(DirectorySector, NULL);

        // Once we have the files "open", we can write the initial version
        // of each file back to disk.  The directory at this point is completely
        // empty; but the bitmap has been changed to reflect the fact that
        // sectors on the disk have been allocated for the file headers and
        // to hold the file data for the directory and bitmap.

        DEBUG('f', "Writing bitmap and directory back to disk.\n");
        freeMap->WriteBack(freeMapFile);     // flush changes to disk
        directory->WriteBack(directoryFile);

        if (DebugIsEnabled('f')) {
            freeMap->Print();
            directory->Print();
        }
        delete freeMap;
        delete directory;
        delete mapHdr;
        delete dirHdr;

    } else {
        // if we are not formatting the disk, just open the files representing
        // the bitmap and directory; these are left open while Nachos is running
        freeMapFile = new OpenFile(FreeMapSector, NULL);
        directoryFile = new OpenFile(DirectorySector, NULL);
    }
}

FileSystem::~FileSystem()
{
    delete freeMapFile;
    delete directoryFile;
}


//----------------------------------------------------------------------
// FileSystem::Create
//  Create a file in the Nachos file system (similar to UNIX create).
//	Since we can't increase the size of files dynamically, we have
//	to give Create the initial size of the file.
//
//	The steps to create a file are:
//    Make sure the file doesn't already exist
//        Allocate a sector for the file header
//    Allocate space on disk for the data blocks for the file
//    Add the name to the directory
//    Store the new file header on disk
//    Flush the changes to the bitmap and the directory back to disk
//
//	Return TRUE if everything goes ok, otherwise, return FALSE.
//
//  Create fails if:
//          file is already in directory
//      no free space for file header
//      no free entry for file in directory
//      no free space for data blocks for the file
//
//  Note that this implementation assumes there is no concurrent access
//	to the file system!
//
//	"name" -- name of file to be created
//	"initialSize" -- size of file to be created
//----------------------------------------------------------------------

bool
FileSystem::Create(const char *name, int initialSize)
{
    BitMap *freeMap;
    FileHeader *hdr;
    int sector;
    bool success;

    DEBUG('f', "Creating file %s, size %d\n", name, initialSize);

    // Expand file name
    char *expandname = ExpandFileName(name);
    char *parentDirectory = DirectoryName(expandname);
    char *filename = FileName(expandname);

    // Get the parent directory
    int parent_sector;

    Directory *directory = GetDirectoryByName(parentDirectory, &parent_sector);

    // Check if parent directory exists
    if (directory == NULL)
    {
        success = FALSE;
    }
    // File is already in directory
    else if (directory->Find(filename) != -1)
    {
        success = FALSE;
    }
    // Name limitation on file
    else if (!CheckNameLimitation(filename))
    {
        success = FALSE;
    }
    else
    {
        freeMap = new BitMap(NumSectors);
        freeMap->FetchFrom(freeMapFile);

        // Find a sector to hold the file header
        sector = freeMap->Find();

        // No free block for file header
        if (sector == -1)
        {
            success = FALSE;
        }
        else if (!directory->Add(filename, sector))
        {
            success = FALSE;	// no space in directory
        }
        else
        {
            hdr = new FileHeader;
            success = TRUE;

            // everthing worked, flush all changes back to disk
            OpenFile *f = new OpenFile(parent_sector, NULL);

            // Write File header on disk
            hdr->WriteBack(sector);

            // Write directory structure on disk
            directory->WriteBack(f);

            // Write free sectors on disk
            freeMap->WriteBack(freeMapFile);

            delete f;
            delete hdr;
        }
        delete freeMap;
    }

    delete directory;
    delete [] parentDirectory;
    delete [] expandname;
    delete [] filename;

    return success;
}

//----------------------------------------------------------------------
// FileSystem::Open
//  Open a file for reading and writing.
//	To open a file:
//    Find the location of the file's header, using the directory
//    Bring the header into memory
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------

OpenFile *
FileSystem::Open(const char *name)
{
    OpenFile *openFile = NULL;
    int sector;

    fileSyncMgr->NewOpenedFile(name);

    DEBUG('f', "Opening file %s\n", name);
    // Expand file name
    char *expandname = ExpandFileName(name);
    char *parentDirectory = DirectoryName(expandname);
    char *filename = FileName(expandname);

    fileSyncMgr->DeleteOpenedFile(name);
    fileSyncMgr->NewOpenedFile(expandname);

    // Get the parent directory
    int parent_sector;

    Directory *directory = GetDirectoryByName(parentDirectory, &parent_sector);

    if (directory == NULL)
    {
        openFile = NULL;
    }
    else
    {
        // Seek file on system
        sector = directory->Find(filename);

        // Filename was found in directory
        if (sector >= 0)
            openFile = new OpenFile(sector, expandname);

        if (openFile == NULL)
            fileSyncMgr->DeleteOpenedFile(expandname);

        delete directory;
    }

    delete [] parentDirectory;
    delete [] expandname;
    delete [] filename;


    return openFile;				// return NULL if not found
}

//----------------------------------------------------------------------
// FileSystem::Remove
//  Delete a file from the file system.  This requires:
//      Remove it from the directory
//      Delete the space for its header
//      Delete the space for its data blocks
//      Write changes to directory, bitmap back to disk
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system.
//
//	"name" -- the text name of the file to be removed
//----------------------------------------------------------------------

int
FileSystem::Remove(const char *name)
{
    BitMap *freeMap;
    FileHeader *fileHdr;
    int sector;

    // Expand file name
    char *expandname = ExpandFileName(name);
    char *parentDirectory = DirectoryName(expandname);
    char *filename = FileName(expandname);

    // Get the parent directory
    int parent_sector;

    // Try to open parent directory
    Directory *directory = GetDirectoryByName(parentDirectory, &parent_sector);

    // Check if file is not already opened
    if (fileSyncMgr->IsOpenedFile(expandname))
    {
        delete [] expandname;
        delete [] parentDirectory;
        delete [] filename;
        if (directory)
            delete directory;
        return -1;
    }

    // If not found
    if (directory == NULL)
    {
        delete [] expandname;
        delete [] parentDirectory;
        delete [] filename;
        return -2;
    }

    // Search for filename
    sector = directory->Find(filename);
    if (sector == -1) {
        delete [] expandname;
        delete [] parentDirectory;
        delete [] filename;
        delete directory;
        return -2;
    }

    // Read file from disk
    fileHdr = new FileHeader;
    fileHdr->FetchFrom(sector);

    // Get free sectors bitmap
    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(freeMapFile);

    // Remove data block of file
    fileHdr->Deallocate(freeMap);
    // Remove header block
    freeMap->Clear(sector);
    // Remove filename from directory
    directory->Remove(filename);

    // Flush freemap to disk
    freeMap->WriteBack(freeMapFile);

    // Write directory modifications to disk
    OpenFile *f = new OpenFile(parent_sector, NULL);
    directory->WriteBack(f);
    delete f;

    delete fileHdr;
    delete directory;
    delete freeMap;
    delete [] expandname;
    delete [] parentDirectory;
    delete [] filename;
    return 0;
}

//----------------------------------------------------------------------
// FileSystem::List
//  List all the files in the file system directory.
//----------------------------------------------------------------------

void
FileSystem::List(const char *dirname)
{
    char *expandname = ExpandFileName(dirname);

    DEBUG('f', "Seek directory %s\n", expandname);

    // Get the parent directory
    Directory *parent = GetDirectoryByName(expandname, NULL);

    DEBUG('f', "Found directory %p\n", parent);
    delete [] expandname;

    if (parent == NULL)
        return;

    parent->List();
    delete parent;
}

//----------------------------------------------------------------------
// FileSystem::Print
//  Print everything about the file system:
//    the contents of the bitmap
//    the contents of the directory
//    for each file in the directory,
//        the contents of the file header
//        the data in the file
//----------------------------------------------------------------------

void
FileSystem::Print()
{
    FileHeader *bitHdr = new FileHeader;
    FileHeader *dirHdr = new FileHeader;
    BitMap *freeMap = new BitMap(NumSectors);
    Directory *directory = new Directory(NumDirEntries);

    printf("Bit map file header:\n");
    bitHdr->FetchFrom(FreeMapSector);
    bitHdr->Print();

    printf("Directory file header:\n");
    dirHdr->FetchFrom(DirectorySector);
    dirHdr->Print();

    freeMap->FetchFrom(freeMapFile);
    freeMap->Print();

    directory->FetchFrom(directoryFile);
    directory->Print();

    delete bitHdr;
    delete dirHdr;
    delete freeMap;
    delete directory;
}

/**
 * Create a directory
 **/
int FileSystem::CreateDirectory(const char* dirname)
{
    // Expand file name
    char *expandname = ExpandFileName(dirname);
    char *parentDirectory = DirectoryName(expandname);
    char *childDirectory = FileName(expandname);

    BitMap *freeMap = new BitMap(NumSectors);

    // Get the parent directory
    int parent_sector;

    Directory *parent = GetDirectoryByName(parentDirectory, &parent_sector);

    int error;

    // Check if parent directory exists
    if (parent == NULL)
        error = -1;

    // Check if name is not already existing
    else if (parent->FileExists(childDirectory) || parent->DirExists(childDirectory))
        error = -2;

    // Name limitation
    else if (!CheckNameLimitation(expandname))
        error = -3;

    // Directory limit
    else if (parent->CheckMaxEntries())
        error = -4;

    // Finally create directory
    else
    {
        // Find free sector
        freeMap->FetchFrom(freeMapFile);

        int sector = freeMap->Find();
        if (sector == -1)
        {
            error = -5;
        }
        else if (!parent->AddDirectory(childDirectory, sector))
        {
            error = -6;
        }
        else
        {
            // Mark sector
            freeMap->Mark(sector);

            // Install directory (alloc space)
            Directory *child = new Directory(NumDirEntries);
            FileHeader *dirHdr = new FileHeader;

            ASSERT(dirHdr->AskForSectors(freeMap, DirectoryFileSize));
            dirHdr->WriteBack(sector);

            // Write directory metadata
            OpenFile *f = new OpenFile(sector, NULL);
            child->WriteBack(f);

            // Update free map
            freeMap->WriteBack(freeMapFile);

            delete f;
            delete child;
            delete dirHdr;
            error = 0;
        }
    }

    // Write back to disk
    if (error == 0)
    {
        OpenFile *f = new OpenFile(parent_sector, NULL);
        parent->WriteBack(f);
        delete f;
    }

    if (parent != NULL)
        delete parent;

    delete [] expandname;
    delete [] parentDirectory;
    delete [] childDirectory;
    delete freeMap;

    return error;
}

/**
 * Convert relative filename to absolute
 *
 * Return a dynamicly allocated name (should be freed)
 **/
char *FileSystem::ExpandFileName(const char* filename)
{
    // Take into account current directory
    std::string filename_s = filename;

    // If relative path
    if (filename_s[0] != '/')
    {
        filename_s = currentThread->GetCurrentDirectory() + filename_s;
    }

    char *cpy = new char[strlen(filename_s.c_str()) + 1];
    char *saveptr = cpy;

    strcpy(cpy, filename_s.c_str());

    if(strcmp(filename_s.c_str(), "..") == 0)
        return cpy;

    char *name = strtok(cpy, "/");
    std::list<std::string> final;

    while (name != NULL)
    {
        if (strcmp(name, "..") == 0 && !final.empty())
        {
            final.pop_back();
        }
        else if (strcmp(name, ".") != 0 && strcmp(name, "") != 0)
        {
            final.push_back(std::string(name));
        }
        // else it's . or //, no change

        name = strtok(NULL, "/");
    }

    // Construct string
    std::string final_string;
    std::list<std::string>::iterator it;

    for (it = final.begin(); it != final.end(); it++)
    {
        final_string += "/";
        final_string += *it;
    }

    delete [] saveptr;

    if (final_string.empty())
        final_string = "/";

    // Convert to char*
    char *res = new char[final_string.size() + 1];
    strcpy(res, final_string.c_str());

    return res;
}

/**
 * Extract dirname from filename
 *
 * Return a dynamicly allocated name (should be freed)
 **/
char *FileSystem::DirectoryName(const char* filename)
{
    // Trick dir name
    std::string st = filename;
    if (st[st.size() - 1] == '/')
        st += "no";

    // For padding, dirname may use bcopy internaly & can cause bad memory
    // access patterns. See http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=691109
    char *cpy = new char[strlen(st.c_str()) + 8];
    bzero(cpy, strlen(st.c_str()) + 8);
    strncpy(cpy, st.c_str(), strlen(st.c_str()) + 1);

    char *result = dirname(cpy);

    char *cpy2 = new char[strlen(result) + 1];
    bzero(cpy2, strlen(result) + 1);
    strcpy(cpy2, result);

    delete [] cpy;

    return cpy2;

}

/**
 * Extract file name from filename
 *
 * Return a dynamicly allocated name (should be freed)
 **/
char *FileSystem::FileName(const char* filename)
{
    // Trick dir name and file name
    std::string st = filename;
    if (st[st.size() - 1] == '/')
    {
        char *cpy = new char[1];
        cpy[0] = '\0';
        return cpy;
    }

    // For padding, basename may use bcopy internaly & can cause bad memory
    // access patterns. See http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=691109
    char *cpy = new char[strlen(st.c_str()) + 8];
    bzero(cpy, strlen(st.c_str()) + 8);
    strncpy(cpy, st.c_str(), strlen(st.c_str()) + 1);

    char *result = basename(cpy);
    char *cpy2 = new char[strlen(result) + 1];
    bzero(cpy2, strlen(result) + 1);
    strcpy(cpy2, result);

    delete [] cpy;

    return cpy2;
}

/**
 * Get directory by name
 *
 * Return a ptr to Directory on success, NULL otherwise
 **/
Directory *FileSystem::GetDirectoryByName(const char* dirname, int *store_sector)
{
    Directory *current = new Directory(NumDirEntries);
    int sector = 0;
    char *cpy = new char[strlen(dirname) + 1];
    strcpy(cpy, dirname);

    char *name = strtok(cpy, "/");

    // Init first directory
    current->FetchFrom(directoryFile);

    // If root, just return current
    if (strcmp(cpy, "/") == 0 || strcmp(cpy, ".") == 0)
    {
        if (store_sector != NULL)
            *store_sector = DirectorySector;
        delete [] cpy;
        return current;
    }

    // Search following inside current directory
    sector = current->Find(name);
    while(sector >= 0)
    {
        name = strtok(NULL, "/");

        if (name == NULL)
        {
            if (store_sector != NULL)
                *store_sector = sector;

            delete current;

            current = Directory::ReadAtSector(sector);
            delete [] cpy;
            return current;
        }

        delete current;
        current = Directory::ReadAtSector(sector);
        sector = current->Find(name);
    }

    if (store_sector != NULL)
        *store_sector = -1;
    delete [] cpy;
    delete current;

    return NULL;

}

/**
 * Check name limitation on dirname
 **/
bool FileSystem::CheckNameLimitation(const char* name)
{
    // Should not be only /
    if (strcmp("/", name) == 0)
        return FALSE;

    // Filename should not be .
    char *filename = FileName(name);
    if (strcmp(".", filename) == 0)
    {
        delete [] filename;
        return FALSE;
    }

    // Filename should not be ..
    if (strcmp("..", filename) == 0)
    {
        delete [] filename;
        return FALSE;
    }

    delete [] filename;

    return TRUE;
}

//----------------------------------------------------------------------
// FileSystem::RemoveDirectory
//  Remove a directory from the file system.  This requires:
//
//	Return TRUE if the directory was deleted, FALSE if the directory wasn't
//	in the file system or empty.
//
//	"name" -- the text name of the file to be removed
//----------------------------------------------------------------------

bool FileSystem::RemoveDirectory(const char *name)
{
    BitMap *freeMap;
    FileHeader *fileHdr;

    // Expand file name
    char *expandname = ExpandFileName(name);
    char *parentDirectory = DirectoryName(expandname);
    char *filename = FileName(expandname);

    // Get the parent directory
    int parent_sector;

    // Try to open parent directory
    Directory *directory = GetDirectoryByName(parentDirectory, &parent_sector);

    // Try open child directory
    int child_sector;
    Directory *child = GetDirectoryByName(expandname, &child_sector);

    // If not found
    if (directory == NULL || child == NULL)
    {
        if (directory != NULL)
            delete directory;
        if (child != NULL)
            delete child;

        delete [] expandname;
        delete [] parentDirectory;
        delete [] filename;
        return FALSE;
    }

    // Check if child directory is empty
    if (!child->IsEmpty())
    {
        delete directory;
        delete child;
        delete [] expandname;
        delete [] filename;
        return FALSE;
    }

    // Delete child directory from disk
    // Read file from disk
    fileHdr = new FileHeader;
    fileHdr->FetchFrom(child_sector);

    // Get free sectors bitmap
    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(freeMapFile);

    // Remove data block of file
    fileHdr->Deallocate(freeMap);

    // Remove header block
    freeMap->Clear(child_sector);

    // Remove directory from directory
    directory->RemoveDirectory(filename);

    // Flush freemap to disk
    freeMap->WriteBack(freeMapFile);

    // Write directory modifications to disk
    OpenFile *f = new OpenFile(parent_sector, NULL);
    directory->WriteBack(f);
    delete f;

    delete child;
    delete fileHdr;
    delete directory;
    delete freeMap;
    delete [] expandname;
    delete [] parentDirectory;
    delete [] filename;
    return TRUE;
}

//----------------------------------------------------------------------
// FileSystem::ListRec
//  List recursively all the files in the file system directory.
//----------------------------------------------------------------------
void FileSystem::ListRec(const char *dirname)
{
    char *expandname = ExpandFileName(dirname);
    std::string name = expandname;

    if (name.size() != 1 && name[name.size()] != '/')
        name += "/";

    // Get the parent directory
    Directory *parent = GetDirectoryByName(name.c_str(), NULL);

    if (parent == NULL)
    {
        printf("Dir %s not found\n", dirname);
        delete [] expandname;
        return;
    }

    parent->PrintRec(name.c_str());
    delete [] expandname;
    delete parent;
}
