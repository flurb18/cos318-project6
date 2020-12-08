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
iNode filesAndDir[];
/* array of file/directory names at index fd. same length as files... */
char* names[];

static int wd; /* Working directory */

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
   int dataPointer;

   assert (filesAndDir[fd] != NULL);
   fp = filesAndDir[fd];
   dataPointer = fp->data;

   if (count == 0) return -1;
   else
   {
      for (i = 0; i < count; i++) 
      {  
         /* return number chars read if EOF reached before count */
         if (dataPointer == (fp->info->size)) return i;
         else (
            block_read(dataPointer, buf[i]);
            dataPointer;
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
   int dataPointer;
   int length;
   char* endChar = '\0';
   
   assert (filesAndDir[fd] != NULL);
   fp = filesAndDir[fd];
   dataPointer = fp->data;
   length = sizeof buf / sizeof *buf;
   
   if (fp->info->size == PAGE_SIZE) return -1;
   else {
      if (fp->info->size < dataPointer)
      {
         block_write(fp->info->size, buf[0]);
         fp->info->size++;
         while (fp->info->size < dataPointer) 
         {
            if (fp->info->size == PAGE_SIZE) return addedBytes;
            block_write(fp->info->size, endchar);
            fp->info->size++;
         }
         beginval = 1;
      }
      else beginval = 0;

      for (i = beginval; i < count; i++) {
         if (fp->info->size == PAGE_SIZE) return addedBytes;

         if (i < length)  block_write(dataPointer, buf[i]);
         else block_write(dataPointer, endchar);

         dataPointer++;
         addedBytes++;
         fp->info->size++;
      }
   }
   
   return addedBytes;
}

int 
fs_lseek( int fd, int offset) {
   iNode fp;
   int size;
 
   assert (filesAndDir[fd] != NULL);
   fp = filesAndDir[fd];
   /*can't set beyong 4096 bytes (max file size) */
   if (offset >= PAGE_SIZE) return -1;

   /* adjusts size of page if offset is before EOF */
   if (offset < fp->info->size) dp->info->size = offset;
  
   /* change data pointer regardless of if offset is beyond EOF */
   fp->data = offset;
   
   return offset;
}

int 
fs_mkdir( char *fileName) {
   int fdchild;
   int fdparent;
   int i;
   iNode directory;
   iNode childFile;
   int length;
   char* childPath;
   char* parentPath;
   char* slash = "/";
   int success;

   fdchild = fs_open(".",FS_O_RDONLY);
   fdparent = fs_open ("..", FS_O_RDONLY);
   childFile = filesAndDir[fdchild];
   childPath = names[fdchild];
   parentPath = names[fdparent];
   length = sizeof filesAndDir / sizeof filesAndDir[0];
   
   for (i = 0; i < length; i++) 
   {
      if (strcmp(names[i], fileName) == 0) return -1;
      if (filesAndDir[i] == NULL) 
      {
         directory->permissions = FS_O_RDONLY;
         directory->data = childFile->data;
         directory->fd = i;
         directory->info->iNodeNo = i;
         directory->info->type = DIRECTORY;
         directory->info->links = 2;
         directory->info->size = childFile->info->size;
         directory->info->numBlocks = childFile->info->numBlocks;
         /* CALL FSLINK ONCE WE FIGURE OUT THAT IMPLEMENTATION */
         return 0;
      }
   }
   return -1;
}

int 
fs_rmdir( char *fileName) {
   int i;
   int length;
   iNode directory;
   
   length = sizeof(names) / sizeof(names[0]);
   
   for(i = 0; i < length; i++) 
   {
      /* if the directory exists */
      if (strcmp(fileName, names[i] == 0)) 
      {
         directory = filesAndDir[i];
         /* error if trying to remove a file using rmdir */
         if (directory->info->type != DIRECTORY) return -1;
         /* error if trying to remove dir with existing links */
         if (directory->info->links > 0) return -1;
         /* free structs and NULL out array items */
         free(directory->info);
         free(directory->data);
         free(directory->nextFile);
         directory[i] = NULL;
         names[i] = NULL;
         return 0;
      }
   }
   /* error if dir does not exist */
   return -1;
}

int 
fs_cd( char *dirName) {
   int i;

   if (strcmp(dirName, ".") == 0) {
      // Already at current directory
      return 0;
   }
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
   int i;

   if (filename == NULL || buf == NULL) {
      return -1;
   }

   for (i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
      if (strcmp(fileName, names[i]) == 0) {
	 // Copy information from inode to fileStat once match is found
	 buf->inodeNo = i;
	 buf->type = filesAndDir[i]->type;
	 buf->links = filesAndDir[i]->links;
	 buf->size = filesAndDir[i]->size;
	 buf->numBlocks = filesAndDir[i]->numBlocks;

	 return 0;
      }
   }
}

