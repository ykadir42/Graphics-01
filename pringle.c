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

	//set background to white
	for(i = 0; i < pixels; i++){
		for(j = 0; j < pixels; j++){
			pic[i][j][0] = 255;
			pic[i][j][1] = 255;
			pic[i][j][2] = 255;
		}
	}

	float x, y;
	for(x = -2; x < 2; x += 0.01){
		for(y = -2; y < 2; y +=0.01){
			if(x * x * x * x / 16 + y * y * y * y / 8 < 1){
				float z = 3 * (x * x - y * y) / 4;
				float partial_x = -2 * x;
				float partial_y = -2 * y;

				//finding the position in the 2D picture given 3D coordinates.
				//Slightly rotated counterclockwise for a better view.
				int column = (int)(pixels / 2 + y * 4 * pixels / 48 - x * 8 * pixels / 48);
				int row = (int)(pixels / 2 - z * pixels / 20 + y * pixels / 16 + x * pixels / 16);

				//Made color proportional to the dot product of the gradient and a vector facing away,
				//creating an effect as if light was coming from the positive x, y, and z direction.
				unsigned char color_scalar = (unsigned char) (16/3 * (-2 * partial_y + partial_x) + 192);
				pic[row][column][0] = color_scalar;
				pic[row][column][1] = color_scalar;
				pic[row][column][2] = color_scalar / 2;

				//rudimentary smoothing
				if(column > 0){
					pic[row][column - 1][0] = color_scalar;
					pic[row][column - 1][1] = color_scalar;
					pic[row][column - 1][2] = color_scalar / 2;
				}
				if(row > 0){
					pic[row - 1][column][0] = color_scalar;
					pic[row - 1][column][1] = color_scalar;
					pic[row - 1][column][2] = color_scalar / 2;
				}
				if(column < pixels - 1){
					pic[row][column + 1][0] = color_scalar;
					pic[row][column + 1][1] = color_scalar;
					pic[row][column + 1][2] = color_scalar / 2;
				}
				if(row < pixels - 1){
					pic[row + 1][column][0] = color_scalar;
					pic[row + 1][column][1] = color_scalar;
					pic[row + 1][column][2] = color_scalar / 2;
				}
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
