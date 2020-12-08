/*
 * Author(s): <Your name here>
 * COS 318, Fall 2019: Project 6 File System.
 * Implementation of a Unix-like file system.
*/
#include "util.h"
#include "common.h"
#include "block.h"
#include "fs.h"
#include "memory.h"

#ifdef FAKE
#include <stdio.h>
#define ERROR_MSG(m) printf m;
#else
#define ERROR_MSG(m)
#endif

/* array of iNodes; feel free to change implementation */
/* not quite sure what the size should be here; don't know how many
   files and directories we can have */
iNode iNodes[];

void 
fs_init( void) {
    block_init();
    /* More code HERE */
}

int
fs_mkfs( void) {
    return -1;
}

int 
fs_open( char *fileName, int flags) {
    return -1;
}

int 
fs_close( int fd) {
    return -1;
}

int 
fs_read( int fd, char *buf, int count) {
   int i;
   iNode fp;

   assert (iNodes[fd] != NULL);
   fp = iNodes[fd];
   dataPointer = fp->data;

   if (count == 0) return -1;
   else
   {
      for (i = 0; i < count; i++) 
      {  
         /* return number chars read if EOF reached before count */
         if (*(fp->data) == EOF) return i;
         else (
            buf[i] = (char)*(fp->data);
            fp->data++;
         )

      }
   }
   return count;
}
    
int 
fs_write( int fd, char *buf, int count) {
   int i;
   iNode fp;
   int addedBytes = 0;
   int starterBits;
   int length;
   
   assert (iNodes[fd] != NULL);
   fp = iNodes[fd];
   length = sizeof buf / sizeof *buf;

   if (fp->size == PAGE_SIZE) return -1;
   else {
      starterBits = (fp->info->size) % 8;
      for (i = 0; i < count; i++) {
         if (fp->size == PAGE_SIZE) return addedBytes;
         if (i < length)  *(fp->data) = buf[i];
         else *(fp->data) = '\0';
         fp->data++;
         starterbits++;
         if (starterbits % 8 == 0)
         {
            addedBytes++;
            fp->info->size++;
         }
      }
   }
   
   return addedBytes;
}

int 
fs_lseek( int fd, int offset) {
    return -1;
}

int 
fs_mkdir( char *fileName) {
    return -1;
}

int 
fs_rmdir( char *fileName) {
    return -1;
}

int 
fs_cd( char *dirName) {
    return -1;
}

int 
fs_link( char *old_fileName, char *new_fileName) {
    return -1;
}

int 
fs_unlink( char *fileName) {
    return -1;
}

int 
fs_stat( char *fileName, fileStat *buf) {
    return -1;
}

