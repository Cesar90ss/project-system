// filehdr.cc
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector,
//
//      Unlike in a real system, we do not keep track of file permissions,
//	ownership, last modification date, etc., in the file header.
//
//	A file header can be initialized in two ways:
//     for a new file, by modifying the in-memory data structure
//       to point to the newly allocated data blocks
//     for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "filehdr.h"

FileHeader::FileHeader() : numBytes(0), numSectors(0)
{
    memset(dataSectors, 0, sizeof(DataBlockInfo) * NumDirect);
}

//----------------------------------------------------------------------
// FileHeader::Allocate
//  Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the bit map of free disk sectors
//----------------------------------------------------------------------

bool
FileHeader::Allocate(BitMap *freeMap, int fileSize)
{
    numBytes = fileSize;
    numSectors  = divRoundUp(fileSize, SectorSize);
    if (freeMap->NumClear() < numSectors)
        return FALSE;		// not enough space

    int sector;

    for (int i = 0; i < numSectors; i++)
    {
        // New indirect block
        if (i % NumIndirect == 0)
        {
            // Initialize structure for data block
            dataSectors[i / NumIndirect] = new DataBlockInfo;
            dataSectors[i / NumIndirect]->data = new DataBlockHdr;

            // Find sector for data block
            sector = freeMap->Find();
            dataSectors[i / NumIndirect]->sector = sector;
        }

        // Write to indirect block
        dataSectors[i / NumIndirect]->data->dataSectors[i % NumIndirect] = freeMap->Find();
    }

    return TRUE;
}

//----------------------------------------------------------------------
// FileHeader::Deallocate
//  De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

void
FileHeader::Deallocate(BitMap *freeMap)
{    int sector;

    for (int i = 0; i < numSectors; i++)
    {
        // Delete indirect block
        if ((i - 1) % NumIndirect == 0)
        {
            // Delete previous data block to disk
            ASSERT(freeMap->Test(dataSectors[(i - 1) / NumIndirect]->sector));
            freeMap->Clear(dataSectors[(i - 1) / NumIndirect]->sector);
        }

        // Delete data block
        ASSERT(freeMap->Test(dataSectors[i / NumIndirect]->data->dataSectors[i % NumIndirect]));
        freeMap->Clear(dataSectors[i / NumIndirect]->data->dataSectors[i % NumIndirect]);
    }
}

//----------------------------------------------------------------------
// FileHeader::FetchFrom
//  Fetch contents of file header from disk.
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void
FileHeader::FetchFrom(int sector)
{
    /*
      fill this with data inside sector :
      synchDisk->ReadSector(sector, (char *)this);

      for each data inside dataSectors
      set new structure with sector = data
      read dataBlocHdr (synchDisk->ReadSector(sector, (char *)this))
    */
    synchDisk->ReadSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
//  Write the modified contents of the file header back to disk.
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void
FileHeader::WriteBack(int sector)
{
    /*
      for each datablockinfo inside data sector
      write data->data inside sector data->sector

      update this->dataSectors with dataSections->sector

      write this inside sector *sector*
    */
    synchDisk->WriteSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
//  Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------

int
FileHeader::ByteToSector(int offset)
{
    /*
      compute the data block with (offset / SectorSize) / SectorSize
      and (offest / SectorSize) % SectorSize
    */
    return(dataSectors[offset / SectorSize]);
}

//----------------------------------------------------------------------
// FileHeader::FileLength
//  Return the number of bytes in the file.
//----------------------------------------------------------------------

int
FileHeader::FileLength()
{
    return numBytes;
}

//----------------------------------------------------------------------
// FileHeader::Print
//  Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------

void
FileHeader::Print()
{
    int i, j, k;
    char *data = new char[SectorSize];

    printf("FileHeader contents.  File size: %d.  File blocks:\n", numBytes);
    for (i = 0; i < numSectors; i++)
        printf("%d ", dataSectors[i]);
    printf("\nFile contents:\n");
    for (i = k = 0; i < numSectors; i++) {
        synchDisk->ReadSector(dataSectors[i], data);
        for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) {
            if ('\040' <= data[j] && data[j] <= '\176')   // isprint(data[j])
                printf("%c", data[j]);
            else
                printf("\\%x", (unsigned char)data[j]);
        }
        printf("\n");
    }
    delete [] data;
}
