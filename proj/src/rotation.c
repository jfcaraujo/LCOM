#include "rotation.h"

uint8_t* (rotate_sprite)(Game* game, xpm_image_t sprite, double angle)
{

  uint16_t width = sprite.width;
  uint16_t height = sprite.height;
  size_t sprite_size = width * height * game->bytesPerPixel;

  uint8_t* new_sprite = (uint8_t*) malloc(sizeof(sprite_size));

  double nx_x = rotate_x(-angle, 1.0, 0.0);
  double nx_y = rotate_y(-angle, 1.0, 0.0);
  double ny_x = rotate_x(-angle, 0.0, 1.0);
  double ny_y = rotate_y(-angle, 0.0, 1.0);

  double initial_x = rotate_x(-angle, -width / 2.0, -height / 2.0) + width / 2.0;
  double initial_y = rotate_y(-angle, -width / 2.0, -height / 2.0) + height / 2.0;

  for(int line = 0; line < height; line++) {
    double x1 = initial_x;
    double y1 = initial_y;
    for(int col = 0; col < width; col++) {

      int old_col = (int) x1;
      int old_line = (int) y1;

      int currentPixel = (col + line * width) * game->bytesPerPixel;
      int currentPixel_Old = (old_col + old_line * width) * game->bytesPerPixel;

      if(old_col < 0 || old_col >= width || old_line < 0 || old_line >= height) {
        *(new_sprite + currentPixel) = (uint8_t) (game->IGNORE_COLOR);
        *(new_sprite + currentPixel + 1) = (uint8_t) (game->IGNORE_COLOR >> 8);
        *(new_sprite + currentPixel + 2) = (uint8_t) (game->IGNORE_COLOR >> 16);
      }
      else {
        *(new_sprite + currentPixel) = (uint8_t) sprite.bytes[currentPixel_Old];
        *(new_sprite + currentPixel + 1) = (uint8_t) sprite.bytes[currentPixel_Old + 1];
        *(new_sprite + currentPixel + 2) = (uint8_t) sprite.bytes[currentPixel_Old + 2];
      }
      
      x1 += nx_x;
      y1 += nx_y;   
    }
    initial_x += ny_x;
    initial_y += ny_y;
  }

  return new_sprite;
}

double rotate_x(double angle, double x_axis, double y_axis) {
  double PI = acos(-1);
  return (x_axis * cos(angle - (PI / 2.0)) + y_axis * (-sin(angle - (PI / 2.0))));
}

double rotate_y(double angle, double x_axis, double y_axis) {
  double PI = acos(-1);
  return (x_axis * sin(angle - (PI / 2.0)) + y_axis * cos(angle - (PI / 2.0)));
}


