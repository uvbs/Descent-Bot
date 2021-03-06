/****************************************************
 * This work is licensed under the Creative
 * Commons Attribution-NonCommercial-ShareAlike
 * 3.0 Unported License. To view a copy of this
 * license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 * or send a letter to Creative Commons, 444
 * Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 ***************************************************/
#ifndef __MATH_H__
#define __MATH_H__

#include <iostream>

using namespace std;

/// This structure supports Long Fixed point math
typedef struct DESCENT_FIXED 
{
   union {
      int raw;
      struct {
	 short hi;
	 unsigned short lo;
      } parts;
   } value;
   float get_Value();
   void set_Value(float f);
   void set_Value(int i);
 private:
   friend struct DESCENT_VERTEX;
   friend class CRdl;
   friend istream &operator>>(istream &input, DESCENT_FIXED &fixed);
   friend ostream &operator<<(ostream &output, DESCENT_FIXED &fixed);
} DESCENT_FIXED;

/// This structure supports Short Fixed point math
typedef struct DESCENT_SHORTFIXED
{
   union {
      short raw;
      struct {
	 signed hi: 4;
	 unsigned lo: 12;
      } parts;
   } value;
 private:
   friend struct DESCENT_VERTEX;
   friend class CRdl;
   friend istream &operator>>(istream &input, DESCENT_SHORTFIXED &fixed);
} DESCENT_SHORTFIXED;

#endif
