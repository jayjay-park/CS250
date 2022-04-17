// Include no new libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// File to use during read operations
#define READABLE_FILE "file_to_read.txt"

#define MAX_BUFFER 1048576  // 2^20

//Declare no new global variables
char write_buf[MAX_BUFFER];
char * wp;
int write_buf_size = 0;
char read_buf[MAX_BUFFER];
char * rp;
int read_buf_size;
int read_count;
int num_write_syscalls;  // count system calls to write()
int num_read_syscalls;   // count system calls to read()

// Write operations will use file descriptor, 1, stdout
int fd_write = 1;

// File descriptor for read operations
int fd_read;

void myWriteByte(char ch)
{
// Use write() to send 1 byte to stdout (file descriptor 1)


}

void myWriteBufSetup(int n)
{


}

void myWriteToBuf(char ch)
{
// Write a char into the buffer


// If buffer is now full
// Use write() to send full buffer to stdout (file descriptor 1)


}

void myWriteBufFlush(void)
{
  if((wp - write_buf) != 0) {
//System call to write a not full buffer
  write(1,write_buf,write_buf_size); }
  num_write_syscalls++;
}

int myReadByte(void)
{
// Create room for another byte to be read into main memory
  char* byte = (char*) malloc(sizeof(char));
// Use read() to read 1 byte from file descriptor fd_read
// Use returned value to handle error and end of file conditions



}

void myReadBufSetup(int n)
{


}

char myReadBuf()
{
  if(read_count <= 0)
  {
// System call to read enough to fill buffer or reach end of file,
// whichever comes first.
// read() returns the number of bytes read
    read_count = 



  }
// Read one byte from the buffer and return it




}


int main()
{
  clock_t begin, end;
  int option, n, temp;
  const char *a="Writing byte by byte\n";
  const char *b="Writing using buffers\n";
  unsigned long i;
  char ch;

  struct stat file_info;
  fd_read = open(READABLE_FILE, O_RDONLY);
  fstat(fd_read,&file_info); // Get number of bytes in file_to_read.txt
  unsigned long int bytes_to_write = file_info.st_size; 
  unsigned long int bytes_to_read = file_info.st_size; 

  while(1)
    {
    printf("\n Read the %ld bytes of %s or",bytes_to_read,READABLE_FILE);
    printf("\n write that same number of bytes to stdout.");
    printf("\n 1 - Write without a buffer");
    printf("\n 2 - Write with a buffer");
    printf("\n 3 - Read without a buffer");
    printf("\n 4 - Read with a buffer");
    printf("\n 5 - Exit");
    printf("\n Enter an option: ");
    scanf("%d", &option);
    switch(option)
    {
      case 1: /* Write without using a buffer */
        num_write_syscalls = 0;

// Timing of unbuffered write and counting write() syscalls starts here
        begin = clock();
        for (i=0;i<bytes_to_write;i++)
        {
          ch = a[i%strlen(a)];
          myWriteByte(ch);
        }
        end = clock();
// Timing of unbuffered write iand counting write() syscalls ends here

        printf("\n Time to write %ld bytes to stdout without buffering",bytes_to_write);
        printf("\n and with using %d system calls to write() is:",num_write_syscalls);
        printf("\n %f secs\n",(double)(end - begin)/CLOCKS_PER_SEC);
        break;

      case 2: /* Write using a buffer */
        printf(" Enter the buffer size in bytes: ");
        scanf("%d", &n);
        if((n < 2) || (n > MAX_BUFFER))
        {
          printf(" Buffer size must be between 2 and 1048576, inclusive.\n\n");
          break;
        }
        myWriteBufSetup(n);
        num_write_syscalls = 0;

// Timing of buffered write and counting write() syscalls starts here
        begin = clock();
        for (i=0;i<bytes_to_write;i++)
        {
          ch = b[i%strlen(b)];
          myWriteToBuf(ch);
        }
        myWriteBufFlush();
        end = clock();
//Timing of buffered write and counting write() syscalls ends here

        printf("\n Time to write %ld bytes using a buffer of",bytes_to_write);
        printf("\n size %d bytes and making %d system calls to write() is:",n,num_write_syscalls);
        printf("\n %f secs\n",(double)(end - begin)/CLOCKS_PER_SEC);
        break;

      case 3: /* Read without using a buffer */
        fd_read = open(READABLE_FILE, O_RDONLY);
        if(fd_read < 0)
        {
          printf("\n Error opening the readable file \n");
          return 1;
        }
        num_read_syscalls = 0;

// Timing of unbuffered read and counting of read() syscalls starts here
        begin = clock();
        for (i=0;i<bytes_to_read;i++)
        {
          int ReadByte = myReadByte();
          if(ReadByte == -1)
          {
            printf(" End of file reached after %ld bytes.",bytes_to_read);
            break;
          }
// Uncomment these lines to print bytes read to stdout, but doing so
// inserts a non-read operation in the timing of myReadByte() execution.
//          else
//          {
//            printf("%c", (char) ReadByte);
//          }
        }
        end = clock();
// Timing of unbuffered read and counting of read() syscalls ends here

        if(close(fd_read))
        {
          printf("\n Error while closing the file \n ");
        }
        printf("\n Time to read file of size %ld bytes without buffering",bytes_to_read);
        printf("\n and with using %d system calls to read() is:",num_read_syscalls);
        printf("\n %f secs\n",(double)(end - begin)/CLOCKS_PER_SEC);
        break;

      case 4: /* Read using a buffer */
        printf(" Enter the buffer size in bytes: ");
        scanf("%d", &n);
        if((n < 2) || (n > MAX_BUFFER))
        {
          printf(" Buffer size must be between 2 and 1048576, inclusive.\n\n");
          break;
        }
        myReadBufSetup(n);
        fd_read = open(READABLE_FILE, O_RDONLY);
        if(fd_read < 0)
        {
          printf("\n Error opening the readable file \n");
          return 1;
        }
        num_read_syscalls = 0;

// Timing of buffered read and counting read() syscalls starts here
        begin = clock();
        for (i=0;i<bytes_to_read;i++)
        {
          char ReadFromBuf = myReadBuf();
          if(ReadFromBuf == -1)
          {
            printf(" End of file reached after %ld bytes.",bytes_to_read);
            break;
          }
// Uncomment these lines to print bytes read to stdout, but doing so
// includes a non-read operation in the timing of myReadBuf() execution.
//          else
//          {
//            printf("%c", ReadFromBuf);
//          }
        }
        end = clock();
// Timing of buffered read and counting of read() syscalls ends here

        if(close(fd_read))
        {
          printf("\n Error while closing the file \n ");
        }
        printf("\n Time to read file of size %ld bytes using a buffer of",bytes_to_read);
        printf("\n size %d bytes and making %d system calls to read() is:",n,num_read_syscalls);
        printf("\n %f secs\n",(double)(end - begin)/CLOCKS_PER_SEC);
        break;

      default:
        return 0;
    }
  }
}
