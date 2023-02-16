#include "rasterizer.h"

using namespace std;

namespace CGL {

  RasterizerImp::RasterizerImp(PixelSampleMethod psm, LevelSampleMethod lsm,
    size_t width, size_t height,
    unsigned int sample_rate) {
    this->psm = psm;
    this->lsm = lsm;
    this->width = width;
    this->height = height;
    this->sample_rate = sample_rate;

    sample_buffer.resize(width * height * sample_rate, Color::White);
  }

  // Used by rasterize_point and rasterize_line
  void RasterizerImp::fill_pixel(size_t x, size_t y, Color c) {
    // TODO: Task 2: You might need to this function to fix points and lines (such as the black rectangle border in test4.svg)
    // NOTE: You are not required to implement proper supersampling for points and lines
    // It is sufficient to use the same color for all supersamples of a pixel for points and lines (not triangles)
      int n = sqrt(sample_rate);
      
      for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
              sample_buffer[(x * n + i) + (y * n + j) * width * n] = c;
          }
      }

  }

  // Rasterize a point: simple example to help you start familiarizing
  // yourself with the starter code.
  //
  void RasterizerImp::rasterize_point(float x, float y, Color color) {
    // fill in the nearest pixel
    int sx = (int)floor(x);
    int sy = (int)floor(y);

    // check bounds
    if (sx < 0 || sx >= width) return;
    if (sy < 0 || sy >= height) return;

    fill_pixel(sx, sy, color);
    return;
  }

  // Rasterize a line.
  void RasterizerImp::rasterize_line(float x0, float y0,
    float x1, float y1,
    Color color) {
    if (x0 > x1) {
      swap(x0, x1); swap(y0, y1);
    }

    float pt[] = { x0,y0 };
    float m = (y1 - y0) / (x1 - x0);
    float dpt[] = { 1,m };
    int steep = abs(m) > 1;
    if (steep) {
      dpt[0] = x1 == x0 ? 0 : 1 / abs(m);
      dpt[1] = x1 == x0 ? (y1 - y0) / abs(y1 - y0) : m / abs(m);
    }

    while (floor(pt[0]) <= floor(x1) && abs(pt[1] - y0) <= abs(y1 - y0)) {
      rasterize_point(pt[0], pt[1], color);
      pt[0] += dpt[0]; pt[1] += dpt[1];
    }
  }

  // Rasterize a triangle.
  void RasterizerImp::rasterize_triangle(float x0, float y0,
    float x1, float y1,
    float x2, float y2,
    Color color) {
      
    // TODO: Task 1: Implement basic triangle rasterization here, no supersampling
    // TODO: Task 2: Update to implement super-sampled rasterization
      
      float ccw = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
      if (ccw < 0) {
          swap(x0, x1); swap(y0, y1);
      }

      int x_min = floor(std::min({x0, x1, x2}));
      int x_max = floor(std::max({x0, x1, x2}));
      int y_min = floor(std::min({y0, y1, y2}));
      int y_max = floor(std::max({y0, y1, y2}));

      float dx0 = x1 - x0;
      float dx1 = x2 - x1;
      float dx2 = x0 - x2;

      float dy0 = y1 - y0;
      float dy1 = y2 - y1;
      float dy2 = y0 - y2;
      
      int n = sqrt(sample_rate);

      for (int i = x_min; i <= x_max; i++) {
          for (int j = y_min; j <= y_max; j++) {

              for (int a = 0; a < n; a++) {
                  for (int b = 0; b < n; b++) {

                      if ((-1 * (i + (a * 2 + 1) * (1.0/(n * 2.0)) - x0) * dy0 + (j + (b * 2 + 1) * (1.0/(n * 2.0)) - y0) * dx0 >= 0) &&
                          (-1 * (i + (a * 2 + 1) * (1.0/(n * 2.0)) - x1) * dy1 + (j + (b * 2 + 1) * (1.0/(n * 2.0)) - y1) * dx1 >= 0) &&
                          (-1 * (i + (a * 2 + 1) * (1.0/(n * 2.0)) - x2) * dy2 + (j + (b * 2 + 1) * (1.0/(n * 2.0)) - y2) * dx2 >= 0)) {
                          sample_buffer[(i * n + a) + (j * n + b) * width * n] = color;
                      }
                  }
              }
          }
      }
      
  }


  void RasterizerImp::rasterize_interpolated_color_triangle(float x0, float y0, Color c0,
    float x1, float y1, Color c1,
    float x2, float y2, Color c2)
  {
    // TODO: Task 4: Rasterize the triangle, calculating barycentric coordinates and using them to interpolate vertex colors across the triangle
      
      float ccw = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
      if (ccw < 0) {
          swap(x0, x1); swap(y0, y1);
      }

      int x_min = floor(std::min({x0, x1, x2}));
      int x_max = floor(std::max({x0, x1, x2}));
      int y_min = floor(std::min({y0, y1, y2}));
      int y_max = floor(std::max({y0, y1, y2}));

      float dx0 = x1 - x0;
      float dx1 = x2 - x1;
      float dx2 = x0 - x2;

      float dy0 = y1 - y0;
      float dy1 = y2 - y1;
      float dy2 = y0 - y2;
      
      int n = sqrt(sample_rate);

      for (int i = x_min; i <= x_max; i++) {
          for (int j = y_min; j <= y_max; j++) {
              
              for (int a = 0; a < n; a++) {
                  for (int b = 0; b < n; b++) {
                      
                      if ((-1 * (i + (a * 2 + 1) * (1.0/(n * 2.0)) - x0) * dy0 + (j + (b * 2 + 1) * (1.0/(n * 2.0)) - y0) * dx0 >= 0) &&
                          (-1 * (i + (a * 2 + 1) * (1.0/(n * 2.0)) - x1) * dy1 + (j + (b * 2 + 1) * (1.0/(n * 2.0)) - y1) * dx1 >= 0) &&
                          (-1 * (i + (a * 2 + 1) * (1.0/(n * 2.0)) - x2) * dy2 + (j + (b * 2 + 1) * (1.0/(n * 2.0)) - y2) * dx2 >= 0)) {
                          
                          float alpha = (-1 * (i + (a * 2 + 1) * (1.0/(n * 2.0)) - x1) * (y2 - y1)
                                         + (j + (b * 2 + 1) * (1.0/(n * 2.0)) - y1) * (x2 - x1))/(-1 * (x0 - x1) * (y2 - y1) + (y0 - y1) * (x2 - x1));
                          float beta = (-1 * (i + (a * 2 + 1) * (1.0/(n * 2.0)) - x2) * (y0 - y2)
                                        + (j + (b * 2 + 1) * (1.0/(n * 2.0)) - y2) * (x0 - x2))/(-1 * (x1 - x2) * (y0 - y2) + (y1 - y2) * (x0 - x2));
                          float gamma = 1 - alpha - beta;
          
                          sample_buffer[(i * n + a) + (j * n + b) * width * n] = alpha * c0 + beta * c1 + gamma * c2;
                      }
                  }
              }
          }
      }
  }

  void RasterizerImp::rasterize_textured_triangle(float x0, float y0, float u0, float v0,
    float x1, float y1, float u1, float v1,
    float x2, float y2, float u2, float v2,
    Texture& tex)
  {
    // TODO: Task 5: Fill in the SampleParams struct and pass it to the tex.sample function.
    // TODO: Task 6: Set the correct barycentric differentials in the SampleParams struct.
    // Hint: You can reuse code from rasterize_triangle/rasterize_interpolated_color_triangle
      
      float ccw = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
      if (ccw < 0) {
          swap(x0, x1); swap(y0, y1);
          swap(u0, u1); swap(v0, v1);
      }

      int x_min = floor(std::min({x0, x1, x2}));
      int x_max = floor(std::max({x0, x1, x2}));
      int y_min = floor(std::min({y0, y1, y2}));
      int y_max = floor(std::max({y0, y1, y2}));

      float dx0 = x1 - x0;
      float dx1 = x2 - x1;
      float dx2 = x0 - x2;

      float dy0 = y1 - y0;
      float dy1 = y2 - y1;
      float dy2 = y0 - y2;

      int p = sqrt(sample_rate);

      for (int i = x_min; i <= x_max; i++) {
          for (int j = y_min; j <= y_max; j++) {

              for (int s = 0; s < p; s++) {
                  for (int t = 0; t < p; t++) {

                      if ((-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x0) * dy0 + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y0) * dx0 >= 0) &&
                          (-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x1) * dy1 + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y1) * dx1 >= 0) &&
                          (-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x2) * dy2 + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y2) * dx2 >= 0)) {

                          float alpha = (-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x1) * (y2 - y1) + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y1) * (x2 - x1))/(-1 * (x0 - x1) * (y2 - y1) + (y0 - y1) * (x2 - x1));
                          float beta = (-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x2) * (y0 - y2) + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y2) * (x0 - x2))/(-1 * (x1 - x2) * (y0 - y2) + (y1 - y2) * (x0 - x2));

                          float m = -1 * (u0 - u1) * (v2 - v1) + (v0 - v1) * (u2 - u1);
                          float n = -1 * (u1 - u2) * (v0 - v2) + (v1 - v2) * (u0 - u2);

                          float a = -1 * (v2 - v1);
                          float b = u2 - u1;
                          float c = u1 * (v2 - v1) - v1 * (u2 - u1) - m * alpha;
                          float d = -1 * (v0 - v2);
                          float e = u0 - u2;
                          float f = u2 * (v0 - v2) - v2 * (u0 - u2) - n * beta;

                          float u = (c * e - b * f)/(a * e - b * d);
                          float v = (c * d - a * f)/(b * d - a * e);
                          Vector2D p_uv = Vector2D(u, v);
                          
                          Vector2D p_dx_uv;
                          if ((-1 * (i + 1 + (s * 2 + 1) * (1.0/(p * 2.0)) - x0) * dy0 + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y0) * dx0 >= 0) &&
                              (-1 * (i + 1 + (s * 2 + 1) * (1.0/(p * 2.0)) - x1) * dy1 + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y1) * dx1 >= 0) &&
                              (-1 * (i + 1 + (s * 2 + 1) * (1.0/(p * 2.0)) - x2) * dy2 + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y2) * dx2 >= 0)) {
                          
                              float alpha_ = (-1 * (i + 1 + (s * 2 + 1) * (1.0/(p * 2.0)) - x1) * (y2 - y1) + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y1) * (x2 - x1))/(-1 * (x0 - x1) * (y2 - y1) + (y0 - y1) * (x2 - x1));
                              float beta_ = (-1 * (i + 1 + (s * 2 + 1) * (1.0/(p * 2.0)) - x2) * (y0 - y2) + (j + (t * 2 + 1) * (1.0/(p * 2.0)) - y2) * (x0 - x2))/(-1 * (x1 - x2) * (y0 - y2) + (y1 - y2) * (x0 - x2));
                              
                              float c_ = u1 * (v2 - v1) - v1 * (u2 - u1) - m * alpha_;
                              float f_ = u2 * (v0 - v2) - v2 * (u0 - u2) - n * beta_;
              
                              float u_ = (c_ * e - b * f_)/(a * e - b * d);
                              float v_ = (c_ * d - a * f_)/(b * d - a * e);
                              p_dx_uv = Vector2D(u_, v_);
                          } else {
                              p_dx_uv = Vector2D(u, v);
                          }
                          
                          Vector2D p_dy_uv;
                          
                          if ((-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x0) * dy0 + (j + 1 + (t * 2 + 1) * (1.0/(p * 2.0)) - y0) * dx0 >= 0) &&
                              (-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x1) * dy1 + (j + 1 + (t * 2 + 1) * (1.0/(p * 2.0)) - y1) * dx1 >= 0) &&
                              (-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x2) * dy2 + (j + 1 + (t * 2 + 1) * (1.0/(p * 2.0)) - y2) * dx2 >= 0)) {
                          
                              float alpha__ = (-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x1) * (y2 - y1) + (j + 1 + (t * 2 + 1) * (1.0/(p * 2.0)) - y1) * (x2 - x1))/(-1 * (x0 - x1) * (y2 - y1) + (y0 - y1) * (x2 - x1));
                              float beta__ = (-1 * (i + (s * 2 + 1) * (1.0/(p * 2.0)) - x2) * (y0 - y2) + (j + 1 + (t * 2 + 1) * (1.0/(p * 2.0)) - y2) * (x0 - x2))/(-1 * (x1 - x2) * (y0 - y2) + (y1 - y2) * (x0 - x2));
                              
                              float c__ = u1 * (v2 - v1) - v1 * (u2 - u1) - m * alpha__;
                              float f__ = u2 * (v0 - v2) - v2 * (u0 - u2) - n * beta__;
              
                              float u__ = (c__ * e - b * f__)/(a * e - b * d);
                              float v__ = (c__ * d - a * f__)/(b * d - a * e);
                              p_dy_uv = Vector2D(u__, v__);
                          } else {
                              p_dy_uv = Vector2D(u, v);
                          }
                          
                          SampleParams sp;
                          sp.p_uv = p_uv;
                          sp.p_dx_uv = p_dx_uv;
                          sp.p_dy_uv = p_dy_uv;

                          Color white = Color::White;
                          
                          int level;
                          Color col;
                          
                          if (lsm == L_NEAREST) {
                              level = round(tex.get_level(sp));
                              
                              if (psm == P_NEAREST) {
                                  col = tex.sample_nearest(p_uv, level);
                              } else {
                                  col = tex.sample_bilinear(p_uv, level);
                              }
                          } else if (lsm == L_ZERO) {
                              level = 0;
                              
                              if (psm == P_NEAREST) {
                                  col = tex.sample_nearest(p_uv, level);
                              } else {
                                  col = tex.sample_bilinear(p_uv, level);
                              }
                          } else {
                              level = tex.get_level(sp);
                              
                              if (psm == P_NEAREST) {
                                  Color col1 = tex.sample_nearest(p_uv, floor(level));
                                  Color col2 = tex.sample_nearest(p_uv, ceil(level));
                                  col = col1 + (level - floor(level)) * (col2 + col1 * (-1));
                              } else {
                                  Color col1 = tex.sample_bilinear(p_uv, floor(level));
                                  Color col2 = tex.sample_bilinear(p_uv, ceil(level));
                                  col = col1 + (level - floor(level)) * (col2 + col1 * (-1));
                              }
                          }
                          
                          sample_buffer[(i * p + s) + (j * p + t) * width * p] = col;
                      }
                  }
              }
          }
      }
  }

  void RasterizerImp::set_sample_rate(unsigned int rate) {
    // TODO: Task 2: You may want to update this function for supersampling support

    this->sample_rate = rate;


    this->sample_buffer.resize(width * height * sample_rate, Color::White);
  }


  void RasterizerImp::set_framebuffer_target(unsigned char* rgb_framebuffer,
    size_t width, size_t height)
  {
    // TODO: Task 2: You may want to update this function for supersampling support
      
      int n = sqrt(sample_rate);

    this->width = width;
    this->height = height;
    this->rgb_framebuffer_target = rgb_framebuffer;


    this->sample_buffer.resize(width * height * sample_rate, Color::White);
  }


  void RasterizerImp::clear_buffers() {
    std::fill(rgb_framebuffer_target, rgb_framebuffer_target + 3 * width * height, 255);
    std::fill(sample_buffer.begin(), sample_buffer.end(), Color::White);
  }


  // This function is called at the end of rasterizing all elements of the
  // SVG file.  If you use a supersample buffer to rasterize SVG elements
  // for antialising, you could use this call to fill the target framebuffer
  // pixels from the supersample buffer data.
  //
  void RasterizerImp::resolve_to_framebuffer() {
    // TODO: Task 2: You will likely want to update this function for supersampling support
      int n = sqrt(sample_rate);

    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        //Color col = sample_buffer[y * width + x];
          float colR = 0.0;
          float colG = 0.0;
          float colB = 0.0;
          
          for (int i = 0; i < n; i++) {
              for (int j = 0; j < n; j++) {
                  colR += sample_buffer[(x * n + i) + (y * n + j) * width * n].r;
                  colG += sample_buffer[(x * n + i) + (y * n + j) * width * n].g;
                  colB += sample_buffer[(x * n + i) + (y * n + j) * width * n].b;
              }
          }
          
          Color col = Color(colR / sample_rate, colG / sample_rate, colB / sample_rate);

        for (int k = 0; k < 3; ++k) {
          this->rgb_framebuffer_target[3 * (y * width + x) + k] = (&col.r)[k] * 255;
        }
      }
    }

  }

  Rasterizer::~Rasterizer() { }


}// CGL
