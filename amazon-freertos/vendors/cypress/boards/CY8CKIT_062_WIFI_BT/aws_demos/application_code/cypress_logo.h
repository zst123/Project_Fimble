#pragma once

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmCypress_Logo_1_BPP_Inv;

/*********************************************************************
*
*       Palette
*
*  Description
*    The following are the entries of the palette table.
*    The entries are stored as a 32-bit values of which 24 bits are
*    actually used according to the following bit mask: 0xBBGGRR
*
*    The lower   8 bits represent the Red   component.
*    The middle  8 bits represent the Green component.
*    The highest 8 bits represent the Blue  component.
*/

// BG, FG
static GUI_CONST_STORAGE GUI_COLOR _ColorsCypress_Logo_1_BPP_Inv[] = {
#if (GUI_USE_ARGB == 0)
		APP_HEX_BG, APP_HEX_LOGO
#else
		(0xFF000000 | APP_HEX_BG), (0xFF000000 | APP_HEX_LOGO)
#endif
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalCypress_Logo_1_BPP_Inv = {
  2,  // Number of entries
  0,  // No transparency
  &_ColorsCypress_Logo_1_BPP_Inv[0]
};

static GUI_CONST_STORAGE unsigned char _acCypress_Logo_1_BPP_Inv[] = {
  ________, _______X, XXXXXXXX, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ________, ____XXXX, XXXXXXXX, XXX_____, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ________, __XXXXXX, XXXXXXXX, XXXXX___, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ________, XXXXXXXX, XXXXXXXX, XXXXXXX_, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  _______X, XXXXXXXX, XXXXXXXX, XXXXXXXX, X_______, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ______XX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XX______, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  _____XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_____, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, _____X__,
  ____XXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXX____, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ____X_X_,
  ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXX___, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, _____X__,
  ___XXXXX, XXXX____, __XXXXXX, XXXXXXXX, XXXXX___, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  __XXXXXX, XX______, ________, XXXXXXXX, XXXXXX__, _______X, XXX____X, X_____XX, __XXXXXX, X_____XX, XXXXX___, ___XXXXX, XXXX____, _XXXX___, _____XXX, X_______,
  __XXX___, ________, _______X, XXXXXXXX, XXXXXX__, _____XXX, XXXX___X, X_____XX, __XXXXXX, XXX___XX, XXXXXXX_, ___XXXXX, XXXX___X, XXXXXX__, ___XXXXX, XX______,
  _XXXXXXX, ________, ________, ________, ____XXX_, ____XXXX, XXXXX__X, XX___XXX, __XXXXXX, XXX___XX, XXXXXXX_, ___XXXXX, XXXX__XX, XXXXXXX_, __XXXXXX, XXX_____,
  _XX_____, ________, ________, ________, ________, ____XXX_, __XXX___, XX___XX_, __XXX___, _XXX__XX, X____XXX, ___XX___, ______XX, X___XXX_, __XXX___, XXX_____,
  ________, ________, ________, ________, ________, ____XXX_, ___XX___, XXX_XXX_, __XXX___, _XXX__XX, X____XXX, ___XX___, ______XX, X____XXX, __XXX___, _XXX____,
  ________, ________, ________, ________, _____XXX, ____XXX_, ___XX___, _XXXXX__, __XXX___, _XXX__XX, X____XXX, ___XX___, ______XX, X____XXX, __XXX___, _XXX____,
  XXXX____, ________, ________, ________, ______XX, ____XXX_, ___XX___, _XXXXX__, __XXX___, _XXX__XX, X____XXX, ___XX___, ______XX, XX______, __XXXX__, ________,
  XXXXXXXX, ________, ________, ________, _____XXX, ____XXX_, ________, _XXXXX__, __XXX___, _XXX__XX, X____XXX, ___XX___, _______X, XXX_____, ___XXXX_, ________,
  XXXXXXXX, X______X, XXX_____, ________, __XXXXXX, ____XXX_, ________, __XXX___, __XXX___, _XXX__XX, X___XXXX, ___XXXXX, XX______, XXXX____, ____XXXX, ________,
  XXXXXXXX, XXXXXXXX, X_______, _______X, XXX_____, ____XXX_, ________, __XXX___, __XXXXXX, XXXX__XX, XXXXXXX_, ___XXXXX, XX______, _XXXX___, _____XXX, X_______,
  XXXXXXXX, XXXXXXXX, XXXX__XX, XX______, ________, ____XXX_, ________, __XXX___, __XXXXXX, XXX___XX, XXXXXX__, ___XXXXX, XX______, __XXXX__, ______XX, XX______,
  XXXXXXXX, XXXXXXXX, XXXXX__X, X___XXXX, XXXXXXXX, ____XXX_, ________, __XXX___, __XXXXXX, X_____XX, X__XXX__, ___XX___, ________, ___XXXX_, _______X, XXX_____,
  XXXXXXXX, XXXXXXXX, XXXXXX__, X__XXXXX, XXXXXXXX, ____XXX_, ___XX___, __XXX___, __XXX___, ______XX, X___XXX_, ___XX___, ________, ____XXX_, ________, XXX_____,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, ___XXXXX, XXXXXXXX, ____XXX_, ___XX___, __XXX___, __XXX___, ______XX, X___XXX_, ___XX___, ______XX, X____XXX, __XXX___, _XXX____,
  _XXXXXXX, XXXXXXXX, XXXXXX__, __XXXXXX, XXXXXXX_, ____XXX_, ___XX___, __XXX___, __XXX___, ______XX, X___XXX_, ___XX___, ______XX, X____XXX, __XXX___, _XXX____,
  _XXXXXXX, XXXXXXXX, XXXXX___, _XXXXXXX, XXXXXXX_, ____XXX_, __XXX___, __XXX___, __XXX___, ______XX, X___XXX_, ___XX___, ______XX, X___XXX_, __XXX___, XXX_____,
  _XXXXXXX, XXXXXXXX, XXXXX___, XXXXXXXX, XXXXXXX_, ____XXXX, XXXXX___, __XXX___, __XXX___, ______XX, X____XXX, ___XXXXX, XXXX__XX, XXXXXXX_, __XXXXXX, XXX_____,
  __XXXXXX, XXXXXXXX, XXXX___X, XXXXXXXX, XXXXXX__, _____XXX, XXXX____, __XXX___, __XXX___, ______XX, X____XXX, ___XXXXX, XXXX___X, XXXXXX__, ___XXXXX, XX______,
  __XXXXXX, XXXXXXXX, XXX___XX, XXXXXXXX, XXXXXX__, _______X, XXX_____, __XXX___, __XXX___, ______XX, X____XXX, ___XXXXX, XXXX____, _XXXX___, _____XXX, X_______,
  ___XXXXX, XXXXXXXX, XX___XXX, XXXXXXXX, XXXXX___, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ___XXXXX, XXXXXXXX, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ____XXXX, XXXXX___, ________, ________, ________, ____XXX_, X___X_XX, __XXX_XX, __XX__XX, X_XX____, X_X__X__, _XXX_XXX, _X___X_X, XX_XX__X, X__XXX_X, _X_X____,
  ________, ________, ________, ________, ________, ____X___, XX_XX_X_, X_X___X_, X_X_X_X_, __X_X___, X_XX_X__, __X__X_X, _XX_XX_X, _X_X_X_X, _X_X_X_X, _X_X____,
  ______XX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XX______, ____XXX_, X_X_X_XX, __XXX_X_, X_X_X_XX, X_X_X___, X_XXXX__, __X__X_X, _X_X_X_X, _X_XX__X, X__X_X_X, _X_X____,
  _______X, XXXXXXXX, XXXXXXXX, XXXXXXXX, X_______, ____X___, X_X_X_X_, X_X___X_, X_X_X_X_, __X_X___, X_X_XX__, __X__X_X, _X_X_X_X, _X_X_X_X, _X_X_X_X, X_XX____,
  ________, XXXXXXXX, XXXXXXXX, XXXXXXX_, ________, ____XXX_, X_X_X_XX, __XXX_XX, __XX__XX, X_XX____, X_X__X__, __X__XXX, _X_X_X_X, XX_X_X_X, _X_XXX_X, ___X____,
  ________, __XXXXXX, XXXXXXXX, XXXXX___, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ________, ____XXXX, XXXXXXXX, XXX_____, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ________, _______X, XXXXXXXX, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________, ________
};

GUI_CONST_STORAGE GUI_BITMAP bmCypress_Logo_1_BPP_Inv = {
  128, // xSize
  40, // ySize
  16, // BytesPerLine
  1, // BitsPerPixel
  _acCypress_Logo_1_BPP_Inv,  // Pointer to picture data (indices)
  &_PalCypress_Logo_1_BPP_Inv   // Pointer to palette
};
