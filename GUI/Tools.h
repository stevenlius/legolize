#ifndef _Tools_h_
#define _Tools_h_

class Tools;

class Tools
{
   public:
      /// Checks whether float value is valid (finite and number)
      /// CPU: O(1)
      /// MEM: O(1)
      /// \param [in] flt Number to be checked
      /// \return true for valid float, false for NaN and infinities
      static bool FloatValid(float flt);
};

#endif