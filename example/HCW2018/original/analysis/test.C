
#include <string>
#include <vector>
#include <cstdio>
#include <utility>

bool stringBreaker(const std::string& sstr,std::vector<std::string>& fstr,const std::string& sep=" ")
{
  std::vector<int> spos;
  std::vector<int> slen;
  size_t ip(0);
  size_t ic(0);
  spos.push_back(0);
  for ( auto fs(sstr.begin()); fs != sstr.end(); ++fs, ++ip ) {
    if ( *fs == ' ' ) { spos.push_back(ip+1); slen.push_back(ic); ic = 0; } else { ++ic; }
  }
  for ( size_t i(0); i<spos.size(); ++i ) {
    if ( i<slen.size() ) {
      fstr.push_back(sstr.substr(spos.at(i),slen.at(i)));
    } else { 
      fstr.push_back(sstr.substr(spos.at(i)));
    }
  }
  return !fstr.empty();
}

int main()
{
  std::string hh = "Wie geht es Euch denn so";
  std::vector<std::string> fields;
  stringBreaker(hh,fields);

  printf("original string \042%s\042\n",hh.c_str());
  printf("fields: ");
  for ( auto fn : fields ) { printf("\042%s\042 ",fn.c_str()); }
  printf("\n");

  return 0;
}
