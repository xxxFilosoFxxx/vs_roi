#include <vector>
#include <ctime>
#include <iostream>

int main ( int argc_p, char ** argv_p ) { 
   std::cout << "\n ---------------------------------\n";
   const std::size_t argumetsAmount_cl = argc_p;
   for ( std::size_t i = 0; i < argumetsAmount_cl; ++i )
      std::cout << "\t" << argv_p[ i ] << "\n";
   std::cout << " ---------------------------------";
   return 0;
}