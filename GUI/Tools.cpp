#include "Tools.h"
#include <float.h>

bool Tools::FloatValid(float flt)
{
   return (flt < FLT_MAX && flt > -FLT_MAX && !(flt >= FLT_MAX) && !(flt <= -FLT_MAX));
}