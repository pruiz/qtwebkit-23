/* ANSI-C code produced by gperf version 2.7.2 */
/* Command-line: gperf -a -L ANSI-C -E -C -c -o -t -k '*' -NfindProp -Hhash_prop -Wwordlist_prop -D -s 2 cssproperties.gperf  */
/* This file is automatically generated from cssproperties.in by makeprop, do not edit */
/* Copyright 1999 W. Bastian */
#include "cssproperties.h"
struct props {
    const char *name;
    int id;
};
/* maximum key range = 988, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash_prop (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993,   0, 993, 993, 993, 993,
      993,   5, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993,  15,  10,   0,
        0,   0, 180,  35,   0,   0,   0,   0,   0,  10,
      315,   0,  65,  25,   0,  25,   0,  20,  30, 240,
      125, 155,   5, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993, 993, 993, 993, 993,
      993, 993, 993, 993, 993, 993
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 32:
        hval += asso_values[(unsigned char)str[31]];
      case 31:
        hval += asso_values[(unsigned char)str[30]];
      case 30:
        hval += asso_values[(unsigned char)str[29]];
      case 29:
        hval += asso_values[(unsigned char)str[28]];
      case 28:
        hval += asso_values[(unsigned char)str[27]];
      case 27:
        hval += asso_values[(unsigned char)str[26]];
      case 26:
        hval += asso_values[(unsigned char)str[25]];
      case 25:
        hval += asso_values[(unsigned char)str[24]];
      case 24:
        hval += asso_values[(unsigned char)str[23]];
      case 23:
        hval += asso_values[(unsigned char)str[22]];
      case 22:
        hval += asso_values[(unsigned char)str[21]];
      case 21:
        hval += asso_values[(unsigned char)str[20]];
      case 20:
        hval += asso_values[(unsigned char)str[19]];
      case 19:
        hval += asso_values[(unsigned char)str[18]];
      case 18:
        hval += asso_values[(unsigned char)str[17]];
      case 17:
        hval += asso_values[(unsigned char)str[16]];
      case 16:
        hval += asso_values[(unsigned char)str[15]];
      case 15:
        hval += asso_values[(unsigned char)str[14]];
      case 14:
        hval += asso_values[(unsigned char)str[13]];
      case 13:
        hval += asso_values[(unsigned char)str[12]];
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      case 11:
        hval += asso_values[(unsigned char)str[10]];
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#endif
const struct props *
findProp (register const char *str, register unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 129,
      MIN_WORD_LENGTH = 3,
      MAX_WORD_LENGTH = 32,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 992
    };

  static const struct props wordlist_prop[] =
    {
      {"color", CSS_PROP_COLOR},
      {"border", CSS_PROP_BORDER},
      {"clear", CSS_PROP_CLEAR},
      {"border-color", CSS_PROP_BORDER_COLOR},
      {"bottom", CSS_PROP_BOTTOM},
      {"size", CSS_PROP_SIZE},
      {"right", CSS_PROP_RIGHT},
      {"height", CSS_PROP_HEIGHT},
      {"border-bottom", CSS_PROP_BORDER_BOTTOM},
      {"border-bottom-color", CSS_PROP_BORDER_BOTTOM_COLOR},
      {"cursor", CSS_PROP_CURSOR},
      {"border-right", CSS_PROP_BORDER_RIGHT},
      {"border-right-color", CSS_PROP_BORDER_RIGHT_COLOR},
      {"top", CSS_PROP_TOP},
      {"clip", CSS_PROP_CLIP},
      {"quotes", CSS_PROP_QUOTES},
      {"border-top", CSS_PROP_BORDER_TOP},
      {"scrollbar-track-color", CSS_PROP_SCROLLBAR_TRACK_COLOR},
      {"border-top-color", CSS_PROP_BORDER_TOP_COLOR},
      {"-khtml-marquee", CSS_PROP__KHTML_MARQUEE},
      {"scrollbar-3dlight-color", CSS_PROP_SCROLLBAR_3DLIGHT_COLOR},
      {"page", CSS_PROP_PAGE},
      {"border-collapse", CSS_PROP_BORDER_COLLAPSE},
      {"scrollbar-highlight-color", CSS_PROP_SCROLLBAR_HIGHLIGHT_COLOR},
      {"left", CSS_PROP_LEFT},
      {"-khtml-marquee-speed", CSS_PROP__KHTML_MARQUEE_SPEED},
      {"max-height", CSS_PROP_MAX_HEIGHT},
      {"float", CSS_PROP_FLOAT},
      {"border-left", CSS_PROP_BORDER_LEFT},
      {"border-style", CSS_PROP_BORDER_STYLE},
      {"border-left-color", CSS_PROP_BORDER_LEFT_COLOR},
      {"list-style", CSS_PROP_LIST_STYLE},
      {"table-layout", CSS_PROP_TABLE_LAYOUT},
      {"border-bottom-style", CSS_PROP_BORDER_BOTTOM_STYLE},
      {"visibility", CSS_PROP_VISIBILITY},
      {"-khtml-box-pack", CSS_PROP__KHTML_BOX_PACK},
      {"opacity", CSS_PROP_OPACITY},
      {"border-right-style", CSS_PROP_BORDER_RIGHT_STYLE},
      {"width", CSS_PROP_WIDTH},
      {"border-width", CSS_PROP_BORDER_WIDTH},
      {"scrollbar-face-color", CSS_PROP_SCROLLBAR_FACE_COLOR},
      {"empty-cells", CSS_PROP_EMPTY_CELLS},
      {"display", CSS_PROP_DISPLAY},
      {"border-top-style", CSS_PROP_BORDER_TOP_STYLE},
      {"-khtml-marquee-style", CSS_PROP__KHTML_MARQUEE_STYLE},
      {"list-style-image", CSS_PROP_LIST_STYLE_IMAGE},
      {"border-bottom-width", CSS_PROP_BORDER_BOTTOM_WIDTH},
      {"border-right-width", CSS_PROP_BORDER_RIGHT_WIDTH},
      {"direction", CSS_PROP_DIRECTION},
      {"scrollbar-arrow-color", CSS_PROP_SCROLLBAR_ARROW_COLOR},
      {"border-top-width", CSS_PROP_BORDER_TOP_WIDTH},
      {"outline", CSS_PROP_OUTLINE},
      {"page-break-before", CSS_PROP_PAGE_BREAK_BEFORE},
      {"outline-color", CSS_PROP_OUTLINE_COLOR},
      {"page-break-after", CSS_PROP_PAGE_BREAK_AFTER},
      {"scrollbar-shadow-color", CSS_PROP_SCROLLBAR_SHADOW_COLOR},
      {"white-space", CSS_PROP_WHITE_SPACE},
      {"unicode-bidi", CSS_PROP_UNICODE_BIDI},
      {"line-height", CSS_PROP_LINE_HEIGHT},
      {"min-height", CSS_PROP_MIN_HEIGHT},
      {"scrollbar-darkshadow-color", CSS_PROP_SCROLLBAR_DARKSHADOW_COLOR},
      {"counter-reset", CSS_PROP_COUNTER_RESET},
      {"margin", CSS_PROP_MARGIN},
      {"border-left-style", CSS_PROP_BORDER_LEFT_STYLE},
      {"max-width", CSS_PROP_MAX_WIDTH},
      {"background", CSS_PROP_BACKGROUND},
      {"margin-bottom", CSS_PROP_MARGIN_BOTTOM},
      {"background-color", CSS_PROP_BACKGROUND_COLOR},
      {"position", CSS_PROP_POSITION},
      {"text-shadow", CSS_PROP_TEXT_SHADOW},
      {"-khtml-marquee-direction", CSS_PROP__KHTML_MARQUEE_DIRECTION},
      {"margin-right", CSS_PROP_MARGIN_RIGHT},
      {"vertical-align", CSS_PROP_VERTICAL_ALIGN},
      {"orphans", CSS_PROP_ORPHANS},
      {"caption-side", CSS_PROP_CAPTION_SIDE},
      {"padding", CSS_PROP_PADDING},
      {"list-style-type", CSS_PROP_LIST_STYLE_TYPE},
      {"border-left-width", CSS_PROP_BORDER_LEFT_WIDTH},
      {"margin-top", CSS_PROP_MARGIN_TOP},
      {"z-index", CSS_PROP_Z_INDEX},
      {"-khtml-flow-mode", CSS_PROP__KHTML_FLOW_MODE},
      {"overflow", CSS_PROP_OVERFLOW},
      {"padding-bottom", CSS_PROP_PADDING_BOTTOM},
      {"-khtml-box-flex", CSS_PROP__KHTML_BOX_FLEX},
      {"letter-spacing", CSS_PROP_LETTER_SPACING},
      {"text-decoration", CSS_PROP_TEXT_DECORATION},
      {"background-image", CSS_PROP_BACKGROUND_IMAGE},
      {"text-decoration-color", CSS_PROP_TEXT_DECORATION_COLOR},
      {"-khtml-box-orient", CSS_PROP__KHTML_BOX_ORIENT},
      {"padding-right", CSS_PROP_PADDING_RIGHT},
      {"border-spacing", CSS_PROP_BORDER_SPACING},
      {"-khtml-box-direction", CSS_PROP__KHTML_BOX_DIRECTION},
      {"-khtml-marquee-repetition", CSS_PROP__KHTML_MARQUEE_REPETITION},
      {"background-repeat", CSS_PROP_BACKGROUND_REPEAT},
      {"page-break-inside", CSS_PROP_PAGE_BREAK_INSIDE},
      {"font", CSS_PROP_FONT},
      {"text-align", CSS_PROP_TEXT_ALIGN},
      {"-khtml-box-lines", CSS_PROP__KHTML_BOX_LINES},
      {"padding-top", CSS_PROP_PADDING_TOP},
      {"widows", CSS_PROP_WIDOWS},
      {"-khtml-box-align", CSS_PROP__KHTML_BOX_ALIGN},
      {"outline-style", CSS_PROP_OUTLINE_STYLE},
      {"font-stretch", CSS_PROP_FONT_STRETCH},
      {"font-size", CSS_PROP_FONT_SIZE},
      {"-khtml-border-vertical-spacing", CSS_PROP__KHTML_BORDER_VERTICAL_SPACING},
      {"margin-left", CSS_PROP_MARGIN_LEFT},
      {"min-width", CSS_PROP_MIN_WIDTH},
      {"outline-width", CSS_PROP_OUTLINE_WIDTH},
      {"-khtml-box-flex-group", CSS_PROP__KHTML_BOX_FLEX_GROUP},
      {"font-size-adjust", CSS_PROP_FONT_SIZE_ADJUST},
      {"-khtml-box-ordinal-group", CSS_PROP__KHTML_BOX_ORDINAL_GROUP},
      {"padding-left", CSS_PROP_PADDING_LEFT},
      {"list-style-position", CSS_PROP_LIST_STYLE_POSITION},
      {"content", CSS_PROP_CONTENT},
      {"text-transform", CSS_PROP_TEXT_TRANSFORM},
      {"font-style", CSS_PROP_FONT_STYLE},
      {"word-spacing", CSS_PROP_WORD_SPACING},
      {"outline-offset", CSS_PROP_OUTLINE_OFFSET},
      {"-khtml-marquee-increment", CSS_PROP__KHTML_MARQUEE_INCREMENT},
      {"text-indent", CSS_PROP_TEXT_INDENT},
      {"background-attachment", CSS_PROP_BACKGROUND_ATTACHMENT},
      {"font-weight", CSS_PROP_FONT_WEIGHT},
      {"background-position", CSS_PROP_BACKGROUND_POSITION},
      {"-khtml-border-horizontal-spacing", CSS_PROP__KHTML_BORDER_HORIZONTAL_SPACING},
      {"font-family", CSS_PROP_FONT_FAMILY},
      {"font-variant", CSS_PROP_FONT_VARIANT},
      {"background-position-x", CSS_PROP_BACKGROUND_POSITION_X},
      {"background-position-y", CSS_PROP_BACKGROUND_POSITION_Y},
      {"counter-increment", CSS_PROP_COUNTER_INCREMENT}
    };

  static const short lookup[] =
    {
       -1,  -1,  -1,  -1,  -1,   0,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,   1,  -1,  -1,  -1,
        2,  -1,   3,  -1,  -1,  -1,   4,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,   5,  -1,  -1,  -1,  -1,  -1,
        6,   7,  -1,   8,  -1,  -1,  -1,  -1,  -1,   9,
       -1,  10,  -1,  -1,  -1,  -1,  -1,  11,  -1,  -1,
       -1,  -1,  -1,  12,  -1,  -1,  -1,  -1,  13,  14,
       -1,  -1,  -1,  -1,  -1,  -1,  15,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  16,  17,  -1,  -1,  -1,
       -1,  18,  -1,  -1,  19,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  20,  -1,  -1,  -1,  -1,  -1,  21,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       22,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  23,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  24,  -1,  -1,  -1,  -1,  -1,
       25,  -1,  -1,  -1,  -1,  26,  -1,  -1,  -1,  -1,
       27,  28,  29,  -1,  -1,  -1,  -1,  30,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  31,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  32,  -1,  33,
       34,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       35,  -1,  36,  37,  -1,  38,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  39,  -1,  -1,  40,  41,  42,  -1,  -1,
       -1,  43,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       44,  45,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  46,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  47,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  48,  -1,  49,  -1,  -1,  -1,
       -1,  50,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  51,  -1,  -1,  -1,  -1,  52,  53,  -1,
       -1,  54,  55,  -1,  -1,  -1,  56,  57,  -1,  -1,
       -1,  58,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       59,  60,  -1,  61,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  62,  -1,  -1,  -1,  -1,  -1,  63,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  64,
       -1,  -1,  -1,  -1,  -1,  65,  -1,  -1,  66,  -1,
       -1,  67,  -1,  68,  -1,  -1,  69,  -1,  -1,  70,
       -1,  -1,  71,  -1,  72,  -1,  -1,  73,  -1,  -1,
       -1,  -1,  74,  -1,  -1,  -1,  -1,  75,  -1,  -1,
       76,  -1,  -1,  -1,  -1,  -1,  -1,  77,  -1,  -1,
       78,  -1,  79,  -1,  -1,  -1,  80,  -1,  81,  -1,
       -1,  -1,  -1,  -1,  82,  83,  -1,  -1,  -1,  84,
       85,  86,  -1,  -1,  -1,  -1,  87,  88,  89,  90,
       91,  -1,  -1,  -1,  -1,  92,  -1,  -1,  -1,  -1,
       -1,  -1,  93,  -1,  -1,  -1,  -1,  94,  -1,  95,
       96,  97,  -1,  -1,  -1,  -1,  98,  -1,  -1,  -1,
       -1,  99,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 100,  -1, 101,  -1,
       -1,  -1, 102,  -1, 103,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      104,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 105,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 106,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 107,  -1,
       -1, 108,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 109,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 110,
       -1,  -1, 111,  -1,  -1,  -1,  -1,  -1,  -1, 112,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 113,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 114, 115,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 116,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 117,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 118,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 119,  -1,  -1,  -1,
       -1, 120,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 121,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 122,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 123,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 124,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 125,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 126,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 127,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 128
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash_prop (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist_prop[index].name;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &wordlist_prop[index];
            }
        }
    }
  return 0;
}
static const char * const propertyList[] = {
"",
"background-color", 
"background-image", 
"background-repeat", 
"background-attachment", 
"background-position", 
"background-position-x", 
"background-position-y", 
"border-collapse", 
"border-spacing", 
"-khtml-border-horizontal-spacing", 
"-khtml-border-vertical-spacing", 
"border-top-color", 
"border-right-color", 
"border-bottom-color", 
"border-left-color", 
"border-top-style", 
"border-right-style", 
"border-bottom-style", 
"border-left-style", 
"border-top-width", 
"border-right-width", 
"border-bottom-width", 
"border-left-width", 
"bottom", 
"-khtml-box-align", 
"-khtml-box-direction", 
"-khtml-box-flex", 
"-khtml-box-flex-group", 
"-khtml-box-lines", 
"-khtml-box-ordinal-group", 
"-khtml-box-orient", 
"-khtml-box-pack", 
"caption-side", 
"clear", 
"clip", 
"color", 
"content", 
"counter-increment", 
"counter-reset", 
"cursor", 
"direction", 
"display", 
"empty-cells", 
"float", 
"font-family", 
"font-size", 
"font-size-adjust", 
"font-stretch", 
"font-style", 
"font-variant", 
"font-weight", 
"height", 
"left", 
"letter-spacing", 
"line-height", 
"list-style-image", 
"list-style-position", 
"list-style-type", 
"margin-top", 
"margin-right", 
"margin-bottom", 
"margin-left", 
"-khtml-marquee", 
"-khtml-marquee-direction", 
"-khtml-marquee-increment", 
"-khtml-marquee-repetition", 
"-khtml-marquee-speed", 
"-khtml-marquee-style", 
"max-height", 
"max-width", 
"min-height", 
"min-width", 
"opacity", 
"orphans", 
"outline-color", 
"outline-offset", 
"outline-style", 
"outline-width", 
"overflow", 
"padding-top", 
"padding-right", 
"padding-bottom", 
"padding-left", 
"page", 
"page-break-after", 
"page-break-before", 
"page-break-inside", 
"position", 
"quotes", 
"right", 
"size", 
"table-layout", 
"text-align", 
"text-decoration", 
"text-decoration-color", 
"text-indent", 
"text-shadow", 
"text-transform", 
"top", 
"unicode-bidi", 
"vertical-align", 
"visibility", 
"white-space", 
"widows", 
"width", 
"word-spacing", 
"z-index", 
"background", 
"border", 
"border-color", 
"border-style", 
"border-top", 
"border-right", 
"border-bottom", 
"border-left", 
"border-width", 
"font", 
"list-style", 
"margin", 
"outline", 
"padding", 
"scrollbar-face-color", 
"scrollbar-shadow-color", 
"scrollbar-highlight-color", 
"scrollbar-3dlight-color", 
"scrollbar-darkshadow-color", 
"scrollbar-track-color", 
"scrollbar-arrow-color", 
"-khtml-flow-mode", 
    0
};
DOMString getPropertyName(unsigned short id)
{
    if(id >= CSS_PROP_TOTAL || id == 0)
      return DOMString();
    else
      return DOMString(propertyList[id]);
};

