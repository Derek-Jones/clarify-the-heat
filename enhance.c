/* enhance.c, 20 Jul 2015 */

/*
 * Flir hackathon
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "enhance.h"

#define trunc floor

kelv_pix im_1_data, im_2_data;

// Write the merged frame here
kelv_comb_pix global_merge;


int x_int_diff,
    y_int_diff;
float x_frac_diff,
      y_frac_diff;

void zero_global_merge(void)
{
int xn, yn;

// printf("global size %d, MAX_X %d, MAX_Y %d\n", sizeof(global_merge), MAX_X_COMBINE, MAX_Y_COMBINE);
for (xn=0; xn < MAX_X_COMBINE; xn++)
   for (yn=0; yn < MAX_Y_COMBINE; yn++)
      global_merge[xn][yn]=0;

}


direction_t x_pixel_shift(direction_t x1,
                          direction_t x2)
{
// (x2-x1)/(X_PIX_FLIR_1/FOV_X)
return radian_to_degree*(x2-x1)*(FOV_X/X_PIX_FLIR_1);
}


direction_t y_pixel_shift(direction_t y1,
                          direction_t y2)
{
// (y2-y1)/(Y_PIX_FLIR_1/FOV_Y)
return radian_to_degree*(y2-y1)*(FOV_Y/Y_PIX_FLIR_1);
}


void map_pixel(int x, int y, kelv_pix im_1, kelv_pix im_2)
{
if ((x-x_int_diff < 0) ||
    (y-y_int_diff < 0))
   return;
global_merge[2*x][2*y]=(im_1[x][y]+
			im_2[x-x_int_diff][y-y_int_diff])/2;
if ((2*x+1 < MAX_X_COMBINE) &&
    (x+1 < X_PIX_FLIR_1))
   global_merge[2*x+1][2*y]=(im_1[x+1][y]+
			im_2[x+1-x_int_diff][y-y_int_diff])/2;
if ((2*y+1 < MAX_Y_COMBINE) &&
    (y+1 < Y_PIX_FLIR_1))
   global_merge[2*x][2*y+1]=(im_1[x][y+1]+
			im_2[x-x_int_diff][y+1-y_int_diff])/2;
if ((2*x+1 < MAX_X_COMBINE) &&
    (x+1 < X_PIX_FLIR_1) &&
    (2*y+1 < MAX_Y_COMBINE) &&
    (y+1 < Y_PIX_FLIR_1))
   global_merge[2*x+1][2*y+1]=(im_1[x+1][y+1]+
			im_2[x+1-x_int_diff][y+1-y_int_diff])/2;
}

// An 'original' pixel is enhanced to four copies of itself
void map_orig_pixel(int x, int y, kelv_type pixel)
{
global_merge[2*x][2*y]=pixel;
if (2*x+1 < MAX_X_COMBINE)
   global_merge[2*x+1][2*y]=pixel;
if (2*y+1 < MAX_Y_COMBINE)
   global_merge[2*x][2*y+1]=pixel;
if ((2*x+1 < MAX_X_COMBINE) &&
    (2*y+1 < MAX_Y_COMBINE))
   global_merge[2*x+1][2*y+1]=pixel;
}


void merge_frame(kelv_pix p1, direction_t p1_x, direction_t p1_y,
                 kelv_pix p2, direction_t p2_x, direction_t p2_y)
{
int xn, yn;
direction_t x_diff=x_pixel_shift(p1_x, p2_x),
            y_diff=y_pixel_shift(p1_y, p2_y);

// printf("x/y diff %f/%f\n", x_diff, y_diff);

if ((abs(x_diff) > MAX_X_DIFF) ||
    (abs(y_diff) > MAX_Y_DIFF))
   return;

x_int_diff=trunc(x_diff);
x_frac_diff=x_diff-x_int_diff;
y_int_diff=trunc(y_diff);
y_frac_diff=y_diff-y_int_diff;

zero_global_merge();

for (xn=0; xn < X_PIX_FLIR_1; xn++)
   {
// printf("%d.", xn);
      for (yn=0; yn < Y_PIX_FLIR_1; yn++)
         if ((xn < x_int_diff) || (yn < y_int_diff))
	    map_orig_pixel(xn, yn, p1[xn][yn]);
	 else
	    {
	    map_pixel(xn, yn, p1, p2);
	    }
   }

return;
}


void print_global(void)
{
int t;
FILE * out_file;

// int xn, yn;
// // for (xn=0; xn < MAX_X_COMBINE; xn++)
// for (xn=0; xn < 10; xn++)
//    {
// //   for (yn=0; yn < MAX_Y_COMBINE; yn++)
//    for (yn=0; yn < 10; yn++)
//       printf("%x,", global_merge[xn][yn]);
//    printf("\n");
//    }

out_file = fopen("image_out.bin", "wb");
if (out_file == NULL)
   {
   printf("Failed to open output file\n");
   return;
   }

t=fwrite(global_merge, 2, MAX_X_COMBINE*MAX_Y_COMBINE, out_file);
fclose(out_file);
}

FILE * open_image(char *filename)
{
FILE * file_ptr = fopen(filename, "rb");

if (file_ptr == NULL)
   {
   printf("Failed to open %s\n", filename);
   return NULL;
   }

// printf("opened %s\n", filename);
return file_ptr;
}


int main(int argc, char **argv)
{
FILE* file_1;
FILE* file_2;
int t;

if (argc < 1)
   {
   printf("Two input files required\n");
   return -1;
   }

file_1=open_image(argv[1]);
t=fread(im_1_data, 2, X_PIX_FLIR_1*Y_PIX_FLIR_1, file_1);
fclose(file_1);

// printf("fread %d, %d, %lu\n", X_PIX_FLIR_1*Y_PIX_FLIR_1, t, sizeof(im_1_data));

file_2=open_image(argv[2]);
t=fread(im_2_data, 2, X_PIX_FLIR_1*Y_PIX_FLIR_1, file_2);
fclose(file_2);

// printf("fread %d, %d, %lu\n", X_PIX_FLIR_1*Y_PIX_FLIR_1, t, sizeof(im_2_data));

merge_frame(im_1_data, 0.0, 0.0,
            im_2_data, 0.0, 0.3);

print_global();
}

