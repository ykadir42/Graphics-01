#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
  int pixels = 500;

  unsigned char pic[pixels][pixels][3];

  int i, j, k;
  
  for(i = 0; i < pixels; i++){
    for(j = 0; i < pixels; i++){
      for(k = 0; i < 3; i++){
	pic[i][j][k] = 0;
      }
    }
  }

  int fd = open("pringle.ppm", O_WRONLY | O_CREAT, 0644);
  for(i = 0; i < pixels; i++){
    for(j = 0; i < pixels; i++){
      for(k = 0; i < 3; i++){
	write(fd, pic[i][j][k], sizeof(unsigned char));
      }
    }
  }
  close(fd);
  
  return 0;
}
