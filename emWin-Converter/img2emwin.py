#!/usr/bin/env python3

## Usage:
## $ python3 img2emwin.py --name "PSoC_Splash_Screen" --input Splash-Screen.png --output output.c

import argparse
from PIL import Image

# Helper functions
def convert_pixel_to_hex_string_8888(r, g, b, a):
	color = a << 24 | r << 16 | g << 8 | b
	color_str = '0x' + (hex(color)[2:]).zfill(8)
	return color_str

def convert_pixel_to_hex_string_565(r, g, b, a):
	color = (r >> 3) << (5+6) | (g >> 2) << 5 | (b >> 3)
	color_str = '0x' + (hex(color)[2:]).zfill(2)
	return color_str

# Export compressed 256-color palette ARGB8888 format
def export_8888_palette256(_im):
	# Limit palette to 256 colors
	_im = _im.convert('P', palette=Image.ADAPTIVE, colors=256)
	_im = _im.convert('RGBA')

	# Get palette colors
	palette_list = set()
	for j in range(height):
		for i in range(width):
			r, g, b, a = _im.getpixel((i, j))
			hex_str = convert_pixel_to_hex_string_8888(r, g, b, a)
			palette_list.add(hex_str)
	palette_list = list(palette_list)
	assert len(palette_list) <= 256

	# Make palette C array
	palette_data = ''
	palette_size = len(palette_list)
	for i, item in enumerate(palette_list):
		if i % 8 == 0 and i != 0:
			palette_data += '\n'
		palette_data += item + ', '

	# Make picture C array
	picture_data = ''
	for j in range(height):
		color_str_list = []
		for i in range(width):
			r, g, b, a = _im.getpixel((i, j))
			hex_str = convert_pixel_to_hex_string_8888(r, g, b, a)
			palette_index = hex(palette_list.index(hex_str))
			color_str_list.append(palette_index)
		color_str_list.append('')

		line_no_chars = len(str(height))
		picture_data += f"  /* Line {str(j).ljust(line_no_chars)} */ {', '.join(color_str_list)}\n"

	template = '''#pragma once

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP @img_name@;

static GUI_CONST_STORAGE GUI_COLOR pPaletteColors[] = {
@palette_data@
};

static GUI_CONST_STORAGE GUI_LOGPALETTE pPalette = {
  @palette_size@, /* No of entries */
  1, /* Transparency */
  &pPaletteColors[0]
};

static GUI_CONST_STORAGE uint8_t pData[] = {
@picture_data@
};

GUI_CONST_STORAGE GUI_BITMAP @img_name@ = {
  @x_size@, // xSize
  @y_size@, // ySize
  @bytes_per_line@, // BytesPerLine
  @bits_per_pixel@, // BitsPerPixel
  (unsigned char *) pData,  // Pointer to picture data
  &pPalette,  // Pointer to palette
  // GUI_DRAW_BMPM8888I
};
'''

	template = template.replace('@img_name@', args.name)
	template = template.replace('@x_size@', str(width))
	template = template.replace('@y_size@', str(height))
	template = template.replace('@bytes_per_line@', str(width))
	template = template.replace('@bits_per_pixel@', str(8))
	template = template.replace('@picture_data@', str(picture_data))
	template = template.replace('@palette_size@', str(palette_size))
	template = template.replace('@palette_data@', str(palette_data))
	return template

# Export uncompressed ARGB8888 format
def export_8888(_im):
	# Extract out pixel color hex
	picture_data = ''
	for j in range(height):
		color_str_list = []
		for i in range(width):
			r, g, b, a = _im.getpixel((i, j))
			hex_str = convert_pixel_to_hex_string_8888(r, g, b, a)
			color_str_list.append(hex_str)
		color_str_list.append('')

		line_no_chars = len(str(height))
		picture_data += f"  /* Line {str(j).ljust(line_no_chars)} */ {', '.join(color_str_list)}\n"

	template = '''#pragma once

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP @img_name@;

static GUI_CONST_STORAGE unsigned long pData[] = {
@picture_data@
};

GUI_CONST_STORAGE GUI_BITMAP @img_name@ = {
  @x_size@, // xSize
  @y_size@, // ySize
  @bytes_per_line@, // BytesPerLine
  @bits_per_pixel@, // BitsPerPixel
  (unsigned char *) pData,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPM8888I
};
'''

	template = template.replace('@img_name@', args.name)
	template = template.replace('@x_size@', str(width))
	template = template.replace('@y_size@', str(height))
	template = template.replace('@bytes_per_line@', str(width * 4))
	template = template.replace('@bits_per_pixel@', str(32))
	template = template.replace('@picture_data@', str(picture_data))
	return template

# Export uncompressed RGB565 format
def export_565(_im):
	# Extract out pixel color hex
	picture_data = ''
	for j in range(height):
		color_str_list = []
		for i in range(width):
			r, g, b, a = _im.getpixel((i, j))
			hex_str = convert_pixel_to_hex_string_565(r, g, b, a)
			color_str_list.append(hex_str)
		color_str_list.append('')

		line_no_chars = len(str(height))
		picture_data += f"  /* Line {str(j).ljust(line_no_chars)} */ {', '.join(color_str_list)}\n"


	template = '''#pragma once

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP @img_name@;

static GUI_CONST_STORAGE uint16_t pData[] = {
@picture_data@
};

GUI_CONST_STORAGE GUI_BITMAP @img_name@ = {
  @x_size@, // xSize
  @y_size@, // ySize
  @bytes_per_line@, // BytesPerLine
  @bits_per_pixel@, // BitsPerPixel
  (unsigned char *) pData,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPM565
};
'''

	template = template.replace('@img_name@', args.name)
	template = template.replace('@x_size@', str(width))
	template = template.replace('@y_size@', str(height))
	template = template.replace('@bytes_per_line@', str(width * 2))
	template = template.replace('@bits_per_pixel@', str(16))
	template = template.replace('@picture_data@', str(picture_data))
	return template


# Parse input / output
parser = argparse.ArgumentParser()
parser.add_argument("-n", "--name", type=str, required=True, help="Name of image")
parser.add_argument("-i", "--input", type=str, required=True, help="Input image file")
parser.add_argument("-o", "--output", type=str, required=True, help="Output C header file")
args = parser.parse_args()

# Convert image to ARGB
im = Image.open(args.input)
width, height = im.size
argb_im = im.convert('RGBA')

# Comment out the function you want
payload = export_8888_palette256(argb_im)
#payload = export_8888(argb_im)
#payload = export_565(argb_im)

# Write to output file
with open(args.output, 'w') as output_file:
    output_file.write(payload)
