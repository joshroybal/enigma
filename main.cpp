#include <iostream>
#include <fstream>
#include <cctype>
#include "enigma.hpp"

std::string assignRotor(char*);

int main(int argc, char* argv[])
{
   // main process
   if (argc < 11) {
      std::cerr << "Usage: " << argv[0] << " I II III A B C X Y Z filename\n";
      return 1;
   }

   // determine rotors from command line arguments
   std::string rtr1 = assignRotor(argv[1]);
   std::string rtr2 = assignRotor(argv[2]);
   std::string rtr3 = assignRotor(argv[3]);
   // test stekker
   // ABCDEFGHIJKLMNOPQRSTUVWXYZ
   // AQRIJFHGDEWLTNSXBCOMZVKPYU
   std::string stkr = "AQRIJFHGDEWLTNSXBCOMZVKPYU";
   // determine rotor positions from command line arguments
   int x = toupper(argv[4][0]) - 65;
   int y = toupper(argv[5][0]) - 65;
   int z = toupper(argv[6][0]) - 65;
   // determine ring settings from command line arguments
   int a = toupper(argv[7][0]) - 65;
   int b = toupper(argv[8][0]) - 65;
   int c = toupper(argv[9][0]) - 65;

   Enigma machine(rtr1, rtr2, rtr3, UKW_B, stkr, x, y, z, a, b, c);

   std::ifstream infile;
   infile.open(argv[10]);
   if (!infile) {
      std::cerr << "Unable to open file " << argv[10] << std::endl;
      return 1;
   }
   int n = 0;
   std::string line;
   std::string outbuf;
   while (std::getline(infile, line)) {
      for (int i = 0; i < line.length(); i++) {
         char ch = line[i];
         if (isalpha(ch)) {
            if (islower(ch))
               ch = char(toupper(ch));
            int idx = ch - 65;
            outbuf += char(machine.cipher(idx) + 65);
            ++n;
            if (n % 5 == 0) {
               if (n % 25 == 0) {
                  std::cout << outbuf << std::endl;
                  outbuf.clear();
               } else
                  outbuf += ' ';
            }
         }
      }
   }
   infile.close();
   if (outbuf.length() > 0)
      std::cout << outbuf << std::endl;
   if (n % 30 == 0)
      std::cout << std::endl;
   return 0;
}

std::string assignRotor(char* arg)
{
   std::string str(arg);
   if (str == "I")
      return ROTOR_I;
   else if (str == "II")
      return ROTOR_II;
   else if (str == "III")
      return ROTOR_III;
   else if (str == "IV")
      return ROTOR_IV;
   else
      return ROTOR_V;
}
