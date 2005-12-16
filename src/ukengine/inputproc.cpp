// -*- coding:unix; mode:c++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
/* Unikey Vietnamese Input Method
 * Copyright (C) 2000-2005 Pham Kim Long
 * Contact:
 *   unikey@gmail.com
 *   UniKey project: http://unikey.org
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include "stdafx.h"
#include <iostream>
#include "inputproc.h"

using namespace std;

unsigned char WordBreakSyms[] = {
	',', ';', ':', '.', '\"', '\'', '!', '?', ' ',
	'<', '>', '=', '+', '-', '*', '/', '\\',
	'_', '~', '`', '@', '#', '$', '%', '^', '&', '(', ')', '{', '}', '[', ']'};

VnLexiName AZLexiUpper[] = 
  {vnl_A, vnl_B, vnl_C, vnl_D, vnl_E, vnl_F, vnl_G, vnl_H, vnl_I, vnl_J,
   vnl_K, vnl_L, vnl_M, vnl_N, vnl_O, vnl_P, vnl_Q, vnl_R, vnl_S, vnl_T,
   vnl_U, vnl_V, vnl_W, vnl_X, vnl_Y, vnl_Z};

VnLexiName AZLexiLower[] =
  {vnl_a, vnl_b, vnl_c, vnl_d, vnl_e, vnl_f, vnl_g, vnl_h, vnl_i, vnl_j,
   vnl_k, vnl_l, vnl_m, vnl_n, vnl_o, vnl_p, vnl_q, vnl_r, vnl_s, vnl_t,
   vnl_u, vnl_v, vnl_w, vnl_x, vnl_y, vnl_z};

UkCharType UkcMap[256];

bool ClassifierTableInitialized = false;

DllExport UkKeyMapping TelexMethodMapping[] = {
    {'Z', vneTone0},
    {'S', vneTone1},
    {'F', vneTone2},
    {'R', vneTone3},
    {'X', vneTone4},
    {'J', vneTone5},
    {'W', vne_telex_w},
    {'A', vneRoof_a},
    {'E', vneRoof_e},
    {'O', vneRoof_o},
    {'D', vneDd},
    {'[', vneCount + vnl_uh},
    {']', vneCount + vnl_oh},
    {'{', vneCount + vnl_Uh},
    {'}', vneCount + vnl_Oh},
    {0, vneNormal}
};

DllExport UkKeyMapping VniMethodMapping[] = {
    {'0', vneTone0},
    {'1', vneTone1},
    {'2', vneTone2},
    {'3', vneTone3},
    {'4', vneTone4},
    {'5', vneTone5},
    {'6', vneRoofAll},
    {'7', vneHook_uo},
    {'8', vneBowl},
    {'9', vneDd},
    {0, vneNormal}
};

DllExport UkKeyMapping VIQRMethodMapping[] = {
    {'0', vneTone0},
    {'\'', vneTone1},
    {'`', vneTone2},
    {'?', vneTone3},
    {'~', vneTone4},
    {'.', vneTone5},
    {'^', vneRoofAll},
    {'+', vneHook_uo},
    {'*', vneHook_uo},
    {'(', vneBowl},
    {'D', vneDd},
    {0, vneNormal}
};

DllExport UkKeyMapping MsViMethodMapping[] = {
    {'5', vneTone2},
    {'%', vneTone2},
    {'6', vneTone3},
    {'^', vneTone3},
    {'7', vneTone4},
    {'&', vneTone4},
    {'8', vneTone1},
    {'*', vneTone1},
    {'9', vneTone5},
    {'(', vneTone5},
    {'1', vneCount + vnl_ab},
    {'!', vneCount + vnl_Ab},
    {'2', vneCount + vnl_ar},
    {'@', vneCount + vnl_Ar},
    {'3', vneCount + vnl_er},
    {'#', vneCount + vnl_Er},
    {'4', vneCount + vnl_or},
    {'$', vneCount + vnl_Or},
    {'0', vneCount + vnl_dd},
    {')', vneCount + vnl_DD},
    {'[', vneCount + vnl_uh},
    {']', vneCount + vnl_oh},
    {'{', vneCount + vnl_Uh},
    {'}', vneCount + vnl_Oh},
    {0, vneNormal}
};

//-------------------------------------------
void SetupInputClassifierTable()
{
  unsigned int c;
  for (c=0; c<256; c++)
    UkcMap[c] = ukcReset;

  for (c = 'a'; c <= 'z'; c++)
    UkcMap[c] = ukcVn;
  for (c = 'A'; c <= 'Z'; c++)
    UkcMap[c] = ukcVn;

  for (c = '0'; c <= '9'; c++)
    UkcMap[c] = ukcNonVn;

  UkcMap[(unsigned char)'j'] = ukcNonVn;
  UkcMap[(unsigned char)'J'] = ukcNonVn;
  UkcMap[(unsigned char)'f'] = ukcNonVn;
  UkcMap[(unsigned char)'F'] = ukcNonVn;
  UkcMap[(unsigned char)'w'] = ukcNonVn;
  UkcMap[(unsigned char)'W'] = ukcNonVn;

  int count = sizeof(WordBreakSyms)/sizeof(unsigned char);
  for (int i = 0; i < count; i++)
    UkcMap[WordBreakSyms[i]] = ukcWordBreak;
}

//-------------------------------------------
void UkInputProcessor::init()
{
  if (!ClassifierTableInitialized) {
    SetupInputClassifierTable();
    ClassifierTableInitialized = true;
  }
  setIM(UkTelex);
}

//-------------------------------------------
int UkInputProcessor::setIM(UkInputMethod im)
{
    m_im = im;
    switch (im) {
        case UkTelex:
            useBuiltIn(TelexMethodMapping);
            break;
        case UkVni:
            useBuiltIn(VniMethodMapping);
            break;
        case UkViqr:
            useBuiltIn(VIQRMethodMapping);
            break;
        case UkMsVi:
            useBuiltIn(MsViMethodMapping);
            break;
        default:
            m_im = UkTelex;
            useBuiltIn(TelexMethodMapping);
    }
    return 1;
}

//-------------------------------------------
int UkInputProcessor::setIM(int map[256])
{
  int i;
  m_im = UkUsrIM;
  for (i=0; i<256; i++)
    m_keyMap[i] = map[i];
  return 1;
}
  

//-------------------------------------------
void UkResetKeyMap(int keyMap[256])
{
  unsigned int c;
  for (c=0; c<256; c++)
    keyMap[c] = vneNormal;
}

//-------------------------------------------
void UkInputProcessor::useBuiltIn(UkKeyMapping *map)
{
    UkResetKeyMap(m_keyMap);
    for (int i=0; map[i].key; i++) {
        m_keyMap[map[i].key] = map[i].action;
        if (map[i].action < vneCount) {
            if (islower(map[i].key)) {
                m_keyMap[toupper(map[i].key)] = map[i].action;
            }
            else if (isupper(map[i].key)) {
                m_keyMap[tolower(map[i].key)] = map[i].action;
            }
        }
    }
}

//-------------------------------------------
void UkInputProcessor::keyCodeToEvent(unsigned int keyCode, UkKeyEvent & ev)
{
  ev.keyCode = keyCode;

  if (keyCode > 255) {
    ev.evType = vneNormal;
    ev.vnSym = IsoToVnLexi(keyCode);
    ev.chType = (ev.vnSym == vnl_nonVnChar)? ukcNonVn : ukcVn;
  }
  else {
    ev.chType = UkcMap[keyCode];
    ev.evType = m_keyMap[keyCode];

    if (ev.evType >= vneTone0 && ev.evType <= vneTone5) {
      ev.tone = ev.evType - vneTone0;
    }

    if (ev.evType >= vneCount) {
      //cout << "Input Processor: shortcut. Key: " << (unsigned char)keyCode 
      //     << " EvType: " << ev.evType << endl; //DEBUG
      ev.chType = ukcVn;
      ev.vnSym = (VnLexiName)(ev.evType - vneCount);
      ev.evType = vneMapChar;
    }
    else {
      ev.vnSym = IsoToVnLexi(keyCode);
    }
  }
}

//-------------------------------------------
UkCharType UkInputProcessor::getCharType(unsigned int keyCode)
{
  if (keyCode > 255)
    return (IsoToVnLexi(keyCode) == vnl_nonVnChar) ? ukcNonVn : ukcVn;
  return UkcMap[keyCode];
}

//-------------------------------------------
void UkInputProcessor::getKeyMap(int map[256])
{
  int i;
  for (i=0; i<256; i++)
    map[i] = m_keyMap[i];
}

//-------------------------------------------
//TODO:
// At the moment we considers character outside the ranges a-z, A-Z
// as non-Vietnamese
// We should handle characters outside those ranges
// to test if they are Vietnamese according to ISO8859-1 (western)
//-------------------------------------------
VnLexiName IsoToVnLexi(int keyCode)
{
  if (keyCode >= 'a' && keyCode <= 'z')
    return AZLexiLower[keyCode - 'a'];
  if (keyCode >= 'A' && keyCode <= 'Z')
    return AZLexiUpper[keyCode - 'A'];
  return vnl_nonVnChar;
}
