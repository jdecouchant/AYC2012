  #include <stdio.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <sys/mman.h>
  #include <limits.h>
  
  int main (int argc, char *argv[])
  {
	  struct stat sb;
	  off_t len;
	  char *p;
	  int fd;
	  
	  
	  fd = open (argv[1], O_RDONLY);
	  
	  
	  p = mmap (0, UINT_MAX, PROT_READ, MAP_SHARED, fd, 0);
	  
	  
	  close (fd) ;
	  
	  while(*p != '\0')
		printf("%c", *p++);
	  printf("XXX");
	  printf("%i", *p++);
	  printf("%c", *p++);
	  printf("%c", *p++);
	  printf("%c", *p++);
	  printf("%c", *p++);
	  
	  return 0;
  } 