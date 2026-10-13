#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>   // std::swap
#include "pixel.h"

// Parse one line "x,y,r,g,b" into a Pixel using find() and substr().
// pos tracks the start of the current field; find(',', pos) locates the
// comma that ends it. stoi/stof (from <string>) convert text to int/float.
Pixel parse_line(const std::string &line)
{
  Pixel p;
  std::string fields[5];
  size_t start = 0;
  for (int i = 0; i < 4; i++)
  {
    size_t comma = line.find(',', start);
    fields[i] = line.substr(start, comma - start);
    start = comma + 1;
  }
  fields[4] = line.substr(start);  // last field has no trailing comma

  p.x = std::stoi(fields[0]);
  p.y = std::stoi(fields[1]);
  p.r = std::stof(fields[2]);
  p.g = std::stof(fields[3]);
  p.b = std::stof(fields[4]);
  return p;
}

// Average r, g, b individually over all pixels and print the results.
void average_colors(std::vector<Pixel> &pixel_list)
{
  double sum_r = 0, sum_g = 0, sum_b = 0;
  for (size_t i = 0; i < pixel_list.size(); i++)
  {
    sum_r += pixel_list[i].r;
    sum_g += pixel_list[i].g;
    sum_b += pixel_list[i].b;
  }
  double n = pixel_list.size();
  std::cout << "Average R: " << sum_r / n << std::endl;
  std::cout << "Average G: " << sum_g / n << std::endl;
  std::cout << "Average B: " << sum_b / n << std::endl;
}

// Flip vertically: in each column of x, the color at y swaps with the
// color at (height-1-y). x and y values stay put so file order is unchanged.
void flip_vertically(std::vector<Pixel> &pixel_list)
{
  // Data is stored column by column, with y varying fastest.
  // Height = number of pixels sharing x == 0 (256 for this file).
  int height = 0;
  while (height < (int)pixel_list.size() && pixel_list[height].x == 0)
    height++;

  int width = pixel_list.size() / height;

  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height / 2; y++)
    {
      Pixel &top = pixel_list[x * height + y];
      Pixel &bottom = pixel_list[x * height + (height - 1 - y)];
      std::swap(top.r, bottom.r);
      std::swap(top.g, bottom.g);
      std::swap(top.b, bottom.b);
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " pixels.dat" << std::endl;
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile.is_open())
  {
    std::cerr << "Could not open " << argv[1] << std::endl;
    return 1;
  }

  std::vector<Pixel> pixel_list;
  std::string line;
  while (std::getline(infile, line))
  {
    if (line.empty()) continue;      // skip blank lines
    pixel_list.push_back(parse_line(line));
  }
  infile.close();

  std::cout << "Read " << pixel_list.size() << " pixels" << std::endl;

  average_colors(pixel_list);
  flip_vertically(pixel_list);

  std::ofstream outfile("flipped.dat");
  for (size_t i = 0; i < pixel_list.size(); i++)
  {
    outfile << pixel_list[i].x << "," << pixel_list[i].y << ","
            << pixel_list[i].r << "," << pixel_list[i].g << ","
            << pixel_list[i].b << "\n";
  }
  outfile.close();

  return 0;
}
