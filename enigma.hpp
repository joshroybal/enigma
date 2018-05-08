#ifndef _ENIGMA_H
#define _ENIGMA_H

#include <string>

static const int C = 26;
// latin alphabet
static const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
// scramblers
static const std::string ROTOR_I = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
static const std::string ROTOR_II = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
static const std::string ROTOR_III = "BDFHJLCPRTXVZNYEIWGAKMUSQO";
static const std::string ROTOR_IV = "ESOVPZJAYQUIRHXLNFTGKDCMWB";
static const std::string ROTOR_V = "VZBRGITYUPSDNHLXAWMJQOFECK";
// reflectors
static const std::string UKW_A = "EJMZALYXVBWFCRQUONTSPIKHGD";
static const std::string UKW_B = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
static const std::string UKW_C = "FVPJIAOYEDRZXWGCTKUQSBNMHL";

// function prototype
int findIndex(char ch, const std::string&);

// class definitions
class Rotor {
   friend class Enigma;
   public:
      Rotor(const std::string& MAP, int pos, int idx) : map_(MAP), offset(pos),
         rngidx(idx) { initialize(MAP); }
      void rotate();
      int cipher(int, const std::string&);
   private:
      int offset;
      int turnidx;
      int rngidx;
      int scrambler[C];
      int inverse[C];
      const std::string map_;
      void initialize(const std::string&);
};

class Reflector {
   public:
      Reflector(const std::string& MAP) { initialize(MAP); }
      int cipher(int);
   private:
      int reflector_[C];
      void initialize(const std::string&);
};

class Plugboard {
   public:
      Plugboard(const std::string& MAP) : stekker_(MAP) { }
      int cipher(int);
   private:
      std::string stekker_;
};

class Enigma {
   public:
      Enigma(const std::string& rtr1, const std::string& rtr2,
         const std::string& rtr3, const std::string& refl,
         const std::string swaps, int x, int y, int z, int a, int b, int c)
         : left(rtr1, x, a), middle(rtr2, y, b), right(rtr3, z, c), ukw(refl), stekker(swaps) { }
      int cipher(int);
   private:
      Rotor left;
      Rotor middle;
      Rotor right;
      Reflector ukw;
      Plugboard stekker;
};

#endif // _ENIGMA_H
