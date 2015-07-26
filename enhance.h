/* enhance.h, 20 Jul 2015 */

/*
 * Flir hackathon
 */

// #define X_PIX_FLIR_1 160
// #define Y_PIX_FLIR_1 120
#define X_PIX_FLIR_1 320
#define Y_PIX_FLIR_1 240

#define FOV_X 56.0
#define FOV_Y 43.64

#define MAX_X_COMBINE (int)(X_PIX_FLIR_1*2)
#define MAX_Y_COMBINE (int)(Y_PIX_FLIR_1*2)

#define MAX_X_DIFF (X_PIX_FLIR_1/2)
#define MAX_Y_DIFF (Y_PIX_FLIR_1/2)

#define radian_to_degree (180/3.14159)

typedef short kelv_type;

typedef kelv_type kelv_pix[X_PIX_FLIR_1][Y_PIX_FLIR_1];
typedef kelv_pix *kelv_pix_ptr;
typedef kelv_type kelv_comb_pix[MAX_X_COMBINE][MAX_Y_COMBINE];
typedef kelv_comb_pix * kelv_comb_ptr;

typedef float direction_t;

extern kelv_comb_pix global_merge;

void merge_frame(kelv_pix p1, direction_t p1_x, direction_t p1_y,
                 kelv_pix p2, direction_t p2_x, direction_t p2_y);


