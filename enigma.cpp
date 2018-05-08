#include "enigma.hpp"

// function returns index of char ch in array - often useful
int findIndex(char ch, const std::string& MAP)
{
   for (int i = 0; i < C; i++)
      if (MAP[i] == ch)
         return(i);
   return 0;   // fail safe
}

// rotor class method definitions

// public methods
// rotation emulation method - counter-clockwise
void Rotor::rotate()
{
   ++offset;
   offset %= C;
}

// character mapping method
int Rotor::cipher(int idx, const std::string& direction)
{
   // adjust input by ring setting
   idx -= rngidx;
   if (idx < 0)
      idx += C;

   idx = (idx + offset) % C;  // adjust input by rotor position

   if (idx < 0)
      idx += C;
   if (direction == "forward")
      idx = (idx + scrambler[idx]) % C;
   if (direction == "backward")
      idx = (idx + inverse[idx]) % C;
   if (idx < 0)
      idx += C;

   // adjust output by rotor position
   idx = (idx - offset) % C;
   if (idx < 0)
      idx += C;

   idx = (idx + rngidx) % C;  // adjust output ring setting

   return idx;
}

// wiring initialization private method
void Rotor::initialize(const std::string& MAP)
{
   // local variables
   char ch;
   int i, idx;
   // method process
   // initialize forward circuit
   for (i = 0, ch = 'A'; ch <= 'Z'; ch++, i++)
      scrambler[i] = MAP[i] - ch;
   // initialize backward circuit
   for (i = 0; i < C; i++) {
      idx = findIndex(ALPHABET[i], MAP);
      inverse[i] = idx - i;
   }
   // determine the turnover notch index
   if (MAP == ROTOR_I)
      turnidx = 'Q' - 65;
   else if (MAP == ROTOR_II)
      turnidx = 'E' - 65;
   else if (MAP == ROTOR_III)
      turnidx = 'V' - 65;
   else if (MAP == ROTOR_IV)
      turnidx = 'J' - 65;
   else
      turnidx = 'Z' - 65;
}

// Reflector class methods

// public methods
// wiring initialization method
void Reflector::initialize(const std::string& MAP)
{
   // local variables
   char ch;
   int i;
   // method process
   // initialize forward circuit
   for (i = 0, ch = 'A'; ch <= 'Z'; ch++, i++)
      reflector_[i] = MAP[i] - ch;
}

// reflector character encoding method
int Reflector::cipher(int idx)
{
   idx += reflector_[idx];
   return idx;
}

// Plugboard/Stekker class definitions
// Plugboard class public methods
int Plugboard::cipher(int idx)
{
   // return stekker_[int(ch)-65];
   return (stekker_[idx] - 65);
}

// Engima class method definitions
// Enigma class public methods

// main cipher method
int Enigma::cipher(int idx)
{
   // rotor stepping chores
   if (middle.offset == middle.turnidx) {
      middle.rotate();
      left.rotate();
   }
   if (right.offset == right.turnidx)
      middle.rotate();
   right.rotate();
   // encode input
   idx = stekker.cipher(idx);
   idx = right.cipher(idx, "forward");
   idx = middle.cipher(idx, "forward");
   idx = left.cipher(idx, "forward");
   idx = ukw.cipher(idx);
   idx = left.cipher(idx, "backward");
   idx = middle.cipher(idx, "backward");
   idx = right.cipher(idx, "backward");
   idx = stekker.cipher(idx);
   return idx;
}
