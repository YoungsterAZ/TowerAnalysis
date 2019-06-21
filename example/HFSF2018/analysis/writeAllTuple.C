
#include "writeTuple.C"

#include <vector>
#include <string>

void writeAllTuple() {

  std::vector<std::string> putags = { "JAPU+INCL", "JAPU+TIME", "CSPU+INCL", "CSPU+TIME", "CSSK+INCL", "CSSK+TIME" };
  std::vector<std::string> stags  = { "topoclus",  "topotowf",  "topotow"                                          };

  for ( auto pu : putags ) { 
    for ( auto st : stags ) { writeTuple(pu,st); }
  }
}
