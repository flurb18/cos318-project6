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
#include <string.h>

#ifdef FAKE
#include <stdio.h>
#define ERROR_MSG(m) printf m;
#else
#define ERROR_MSG(m)
#endif

// need to change
enum {
  MAX_INODES = 100;
}

/* array of iNodes; feel free to change implementation */
/* not quite sure what the size should be here; don't know how many
   files and directories we can have */
iNode filesAndDir[MAX_INODES];
/* array of file/directory names at index fd. same length as files... */
char* names[MAX_INODES];

static int wd; /* Working directory */

void 
fs_init( void) {
  block_init();
  /* More code HERE */
  char buf[BLOCK_SIZE];
  block_read(0, buf);
  if (strcmp(buf, "magic") == 0) {
    // disk is already formatted
  } else {
    fs_mkfs();
  }
}

int
fs_mkfs( void) {
  block_write(0, "magic");
  int i;
  for (i = 0; i < MAX_INODES; i++) {
    filesAndDir[i].permissions = 0;
    filesAndDir[i].fd = i;
    filesAndDir[i].type = FREE_INODE;
    filesAndDir[i].size = 0;
    filesAndDir[i].links = 0;
    filesAndDir[i].numBlocks = 0;
  }
  wd = fs_mkdir("/");
}

int 
fs_open( char *fileName, int flags) {
  int i;
  for (i = 0; i < MAX_INODES; i++) {
    if (strcmp(fileName, names[i]) == 0) {
      filesAndDir[i].permissions = flags;
      return i;
    }
    if (filesAndDir[i].type == FREE_INODE) return i;
  }
  return -1;
}

int 
fs_close( int fd) {
  filesAndDir[fd].type = FREE_INODE;
  filesAndDir[fd].permissions = 0;
  filesAndDir[fd].size = 0;
  filesAndDir[fd].link = 0;
  filesAndDir[fd].numBlocks = 0;
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
          if (dataPointer == (fp->size)) return i;
          else {
                block_read(dataPointer, buf[i]);
                dataPointer;
          }

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
   
  if (fp->size == PAGE_SIZE) return -1;
  else {
    if (fp->size < dataPointer)
      {
        block_write(fp->size, buf[0]);
        fp->size++;
        while (fp->size < dataPointer) 
          {
            if (fp->size == PAGE_SIZE) return addedBytes;
            block_write(fp->size, endchar);
            fp->size++;
          }
        beginval = 1;
      }
    else beginval = 0;

    for (i = beginval; i < count; i++) {
      if (fp->size == PAGE_SIZE) return addedBytes;

      if (i < length)  block_write(dataPointer, buf[i]);
      else block_write(dataPointer, endchar);

      dataPointer++;
      addedBytes++;
      fp->size++;
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
  if (offset < fp->size) fp->size = offset;
  
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

  wd = fs_open(fileName, FS_O_RDONLY);
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
          directory->iNodeNo = i;
          directory->type = DIRECTORY;
          directory->links = 2;
          directory->size = childFile->size;
          directory->numBlocks = childFile->numBlocks;
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
          if (directory->type != DIRECTORY) return -1;
          /* error if trying to remove dir with existing links
             besdies link to self and parent */
          if (directory->links > 2) return -1;
          /* free structs and NULL out array items */
          wd = cd("..");
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

