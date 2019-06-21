// -*- c++ c-*-
#ifndef ANALYSISBANNER_H
#define ANALYSISBANNER_H

#include "AnalysisUtils.h"

#include <vector>
#include <string>

namespace Analysis {
  ///@brief Banner printout 
  namespace Banner { 
    ///@brief Filter
    namespace Filter {
      ///@brief Calo timing
      namespace CaloTiming {
	static const std::vector<std::string> text = { " +-------------------------------------------------------------------------------------------+",
						       "+-------------------------------------------------------------------------------------------+|",
						       "|  ###   ###  #      ###  ##### ### #   # ### #   #  #### ##### ### #     ##### ##### ####  ||",
						       "| #   # #   # #     #   #   #    #  ## ##  #  ##  # #     #      #  #       #   #     #   # ||",
						       "| #     ##### #     #   #   #    #  # # #  #  # # # # ### ###    #  #       #   ###   ####  ||",
						       "| #   # #   # #     #   #   #    #  #   #  #  #  ## #   # #      #  #       #   #     #  #  ||",
						       "|  ###  #   # #####  ###    #   ### #   # ### #   #  ###  #     ### #####   #   ##### #   # |+",
						       "+-------------------------------------------------------------------------------------------+" };
      }
    }
    ///@brief Topo-cluster
    namespace TopoCluster {
      ///@brief Banner data
      static const std::vector<std::string> text = { " +-------------------------------------------------------------------+",
						     "+-------------------------------------------------------------------+|",
						     "| #####  ###  ####   ###   ###  #     #   #  #### ##### ##### ####  ||",
						     "|   #   #   # #   # #   # #   # #     #   # #       #   #     #   # ||",
						     "|   #   #   # ####  #   # #     #     #   #  ###    #   ###   ####  ||",
						     "|   #   #   # #     #   # #   # #     #   #     #   #   #     #  #  ||",
						     "|   #    ###  #      ###   ###  #####  ###  ####    #   ##### #   # |+",
						     "+-------------------------------------------------------------------+" };
    }
    ///@brief Topo-tower (standard @f$ \Delta\eta\times\Delta\phi = 0.1\times 0.1 @f$ )
    namespace TopoTower {
      static const std::vector<std::string> text = { " +---------------------------------------------------------------------------------------+",
						     "+---------------------------------------------------------------------------------------+|",
						     "| #####  ###  ####   ###  #####  ###  #   # ##### ####     ###     #  #   #  ###     #  ||",
						     "|   #   #   # #   # #   #   #   #   # #   # #     #   #   #  ##   ##   # #  #  ##   ##  ||",
						     "|   #   #   # ####  #   #   #   #   # # # # ###   ####    # # #    #    #   # # #    #  ||",
						     "|   #   #   # #     #   #   #   #   # # # # #     #  %    ##  #    #   # #  ##  #    #  ||",
						     "|   #    ###  #      ###    #    ###   ###  ##### #   %    ###  # ### #   #  ###  # ### |+",
						     "+---------------------------------------------------------------------------------------+" };
    }
    ///@brief Topo-tower (fine @f$ \Delta\eta\times\Delta\phi = 0.05\times 0.05 @f$ )
    namespace TopoTowerFine {
      static const std::vector<std::string> text = { " +-------------------------------------------------------------------------------------------------------+",
						     "+-------------------------------------------------------------------------------------------------------+|",
						     "| #####  ###  ####   ###  #####  ###  #   # ##### ####     ###     ###  ##### #   #  ###     ###  ##### ||",
						     "|   #   #   # #   # #   #   #   #   # #   # #     #   #   #  ##   #  ## #      # #  #  ##   #  ## #     ||",
						     "|   #   #   # ####  #   #   #   #   # # # # ###   ####    # # #   # # # ####    #   # # #   # # # ####  ||",
						     "|   #   #   # #     #   #   #   #   # # # # #     #  %    ##  #   ##  #     #  # #  ##  #   ##  #     # ||",
						     "|   #    ###  #      ###    #    ###   ###  ##### #   %    ###  #  ###  ####  #   #  ###  #  ###  ####  |+",
						     "+-------------------------------------------------------------------------------------------------------+" };
    }
    ///@name Printers
    ///
    /// A collection of functions printing banners or list of strings.
    /// A line-feed and carriage return is printed after each string.
    ///@{
    ///@param module name of the module invoking the print
    ///@param banner lines of text
    static void print(const std::string& module,const std::vector<std::string>& banner)
    { for ( auto ln : banner ) { PRINT_INFO( module, " %s",ln.c_str() ); } }                         ///< Print any list of strings
    static void printTopoCluster(const std::string& module)   { print(module,TopoCluster::text);   } ///< Print topo-cluster banner
    static void printTopoTower(const std::string& module)     { print(module,TopoTower::text);     } ///< Print topo-tower ( @f$ \Delta\eta\times\Delta\phi = 0.1\times 0.1 @f$ )  banner
    static void printTopoTowerFine(const std::string& module) { print(module,TopoTowerFine::text); } ///< Print topo-tower ( @f$ \Delta\eta\times\Delta\phi = 0.05\times 0.05 @f$ ) banner 
    ///@}
  } 
}
#endif
