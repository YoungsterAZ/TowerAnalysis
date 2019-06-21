
#include "XMLData.h"


XMLData::XMLData(const std::string& blockName)
  : _eventCtr(0)
  , _blockName(blockName)
  , _nbytes(0)
  , _records(0)
{ _event.resize(250000); }

XMLData::~XMLData()
{ }

