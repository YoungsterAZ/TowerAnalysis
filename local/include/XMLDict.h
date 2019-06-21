// -- c++ --
#ifndef XMLDICT_H
#define XMLDICT_H

#include <string>

namespace XMLDict {
  namespace Format {
    // event tags
    static const std::string eventBegin  = "<event id=\042%09i\042 type=\042%s\042 eunit=\042%s\042>";
    static const std::string eventBeginFront = "<event id=\042%i\042 type=\042%s\042 eunit=\042%s\042";
    static const std::string eventBeginBack  = " data_records=\042%zu\042 lines=\042%zu\042>";
    static const std::string eventEnd    = "</event>";
    // event variables 
    static const std::string rhoBegin    = "  <rho scale=\042%s\042 type=\042%s\042 rows=\042%i\042 columns=\042%i\042 coltitles=\042%s\042>";
    static const std::string rhoEnd      = "  </rho>";
    static const std::string muBegin     = "  <mu>";
    static const std::string muEnd       = "  </mu>";
    static const std::string npvBegin    = "  <npv>";
    static const std::string npvEnd      = "  </npv>";
    static const std::string weightBegin = "  <weight type=\042%s\042>";
    static const std::string weightEnd   = "  </weight>";
    // vertex variables
    static const std::string vtxBegin    = "  <vertex type=\042%s\042 rows=\042%i\042 columns=\042%i\042 coltitles=\042%s\042>";
    static const std::string vtxEnd      = "  </vertex>";
    static const std::string vtxFmt      = "    %7.3f %7.3f %7.3f %4i";
    // signals
    static const std::string signalBegin = "  <signal type=\042%s\042 blocks=\042%s\042>";
    static const std::string signalEnd   = "  </signal>";
    // kinematics
    static const std::string kineBegin   = "    <kine rows=\042%i\042 columns=\042%zu\042 coltitles=\042%s\042>";
    static const std::string kineEnd     = "    </kine>";
    // moments
    static const std::string momentBegin = "    <moment rows=\042%i\042 columns=\042%zu\042 coltitles=\042%s\042>";
    static const std::string momentEnd   = "    </moment>";
    // truth
    static const std::string truthBegin  = "  <truth type=\042%s\042>";
    static const std::string truthEnd    = "  </truth>";
    // jets
    static const std::string jetBegin    = "    <jet type=\042%s\042algorithm=\042%s\042 radius=\042%3.1f\042 rows=\042%i\042 columns=\042%i\042 coltitles=\042%s\042>";
    static const std::string jetEnd      = "    </jet>";
  } // Format
} // XMLDict
#endif
