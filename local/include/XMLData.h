// -*- c++ -*-
#ifndef XMLDATA_H
#define XMLDATA_H

#include <string>
#include <vector>
#include <cstdio>

#include "XMLDict.h"

#define CHECK_SIZE( C, N )			\
  if ( C->size() != N ) {			\
  std::string msg("XMLWriter - ERROR inconsistent data ");	\
  msg += #C;							\
  msg += "->size = %uz, should be %uz\n";			\
  printf(msg.c_str(), C->size(), N);				\
  return -1;							\
  }

class XMLData
{
public:

  XMLData(const std::string& blockName="XMLData");
  virtual ~XMLData();

  template<class T>
  long int fillEvent(T* data,long int jentry);

  const std::vector<std::string>& getEvent() const;

protected:

  std::vector<std::string> _event;
  int                      _eventCtr;
  std::string              _blockName;
  long int                 _nbytes;
  int                      _records;

  int _addToEvt(const char* buffer);
  int _openEvt();
  int _closeEvt();
  int _openBlock(const char* buffer);
  int _closeBlock(const char* buffer);
  int _openBlock(const std::string& str);
  int _closeBlock(const std::string& str);
};

inline const std::vector<std::string>& XMLData::getEvent() const
{ return _event; }

inline int XMLData::_addToEvt(const char* buffer) 
{ _event.push_back(std::string(buffer)); return static_cast<int>(_event.size()); }

inline int XMLData::_openEvt()
{ 
  static char buffer[2048];
  _event.clear();
  sprintf(buffer,XMLDict::Format::eventBeginFront.c_str(),_eventCtr,"FullSim","MeV");
  _addToEvt(buffer);
  _eventCtr++;
  return _eventCtr-1;
}

inline int XMLData::_closeEvt()
{
  static char buffer[2048];
  sprintf(buffer,XMLDict::Format::eventBeginBack.c_str(),_records,_event.size()-1);
  _event.front() += std::string(buffer);
  _addToEvt(XMLDict::Format::eventEnd.c_str());
  return _eventCtr;
}

inline int XMLData::_openBlock(const char* buffer)
{ return _addToEvt(buffer); }
inline int XMLData::_closeBlock(const char* buffer)
{ return _addToEvt(buffer); }

inline int XMLData::_openBlock(const std::string& str)
{ return _addToEvt(str.c_str()); }
inline int XMLData::_closeBlock(const std::string& str)
{ return _addToEvt(str.c_str()); }

#include "XMLData.icc"
#endif
