#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))

int main(int argc, char *argv[])
{
    int i;
    int fd;
    int result;
    int *map;  /* mmapped array of int's */

    fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    perror("creating file");
    assert(fd != -1);

    result = lseek(fd, FILESIZE-1, SEEK_SET);
    assert(fd != -1);
    
    result = write(fd, "", 1);
    perror("Strech file");
    assert(fd != -1);
    
    map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	perror("mapping file with mmap");
	
    if (map == MAP_FAILED) {
		close(fd);
		exit(EXIT_FAILURE);
    }
    
    // writing to mmap
    for (i = 1; i <=NUMINTS; ++i) {
		map[i] = 2 * i;
    }

    for (i = 1; i <=NUMINTS; ++i) {
		printf("reading mmap: %d: %d\n", i, map[i]);
    }

    munmap(map, FILESIZE);
	perror("Unmapping");
	
    close(fd);
    perror("closed file");
    exit(EXIT_SUCCESS);    
}
