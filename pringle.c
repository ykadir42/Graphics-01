#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>

int main(){
	int pixels = 600;

	//array of pixels' colors
	unsigned char pic[pixels][pixels][3];

	int i, j, k;

	//set background to black
	for(i = 0; i < pixels; i++){
		for(j = 0; j < pixels; j++){
			pic[i][j][0] = 0;
			pic[i][j][1] = 0;
			pic[i][j][2] = 0;
		}
	}

	float x, y;
	for(x = -2; x < 2; x += 0.01){
		for(y = -2; y < 2; y +=0.01){
			float z = x * x - y * y;
			float partial_y = -2 * y;

			//finding the position in the 2D picture given 3D coordinates.
			//SLightly rotated counterclockwise for a better view.
			int column = (int)(pixels / 2 + y * 5 * pixels / 48 - x * 7 * pixels / 48);
			int row = (int)(pixels / 2 - z * pixels / 20 + y * pixels / 16 + x * pixels / 16);

			//Made color proportional to the dot product of the gradient and the negative y unit vector,
			//creating an effect as if light was coming from the positive y and z direction.
			unsigned char color_scalar = (unsigned char) 16 * -1 * (partial_y) + 128;
			pic[row][column][0] = color_scalar;
			pic[row][column][1] = color_scalar;
			pic[row][column][2] = color_scalar / 4;

			//rudimentary smoothing
			if(column > 0){
				pic[row][column - 1][0] = color_scalar;
				pic[row][column - 1][1] = color_scalar;
				pic[row][column - 1][2] = color_scalar / 4;
			}
			if(row > 0){
				pic[row - 1][column][0] = color_scalar;
				pic[row - 1][column][1] = color_scalar;
				pic[row - 1][column][2] = color_scalar / 4;
			}
			if(column < pixels - 1){
				pic[row][column + 1][0] = color_scalar;
				pic[row][column + 1][1] = color_scalar;
				pic[row][column + 1][2] = color_scalar / 4;
			}
			if(row < pixels - 1){
				pic[row + 1][column][0] = color_scalar;
				pic[row + 1][column][1] = color_scalar;
				pic[row + 1][column][2] = color_scalar / 4;
			}
		}
	}

	//writing the ppm file
	int fd = open("pringle.ppm", O_WRONLY | O_CREAT, 0644);
	char s[100];
	sprintf(s, "P3\n%d %d \n255\n", pixels, pixels);
	write(fd, s, strlen(s));
	for(i = 0; i < pixels; i++){
		for(j = 0; j < pixels; j++){
			for(k = 0; k < 3; k++){
				char c[4];
				sprintf(c, "%03d", pic[i][j][k]);
				write(fd, c, 3*sizeof(char));
				write(fd, " ", sizeof(" "));
			}
			write(fd, "\n", sizeof("\n"));
		}
	}
	close(fd);

	return 0;
}
