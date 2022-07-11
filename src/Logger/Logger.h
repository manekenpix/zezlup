#ifndef LOGGER_H
#define LOGGER_H

#include "../types.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

using namespace std;

class Logger
{
  const char* ZEZLUP_LOG_LEVEL = "zezlup_log_level";
  const char* RESET = "\033[0m";
  const char* RED = "\033[31m";
  const char* GREEN = "\033[32m";
  const char* YELLOW = "\033[33m";
  const char* BLUE = "\033[34m";

  const char* infoHeader = "[info] ";
  const char* debugHeader = "[debug] ";
  const char* errorHeader = "[error] ";

  stringstream doubleStream;

  string log, logLevel;
  const vector<string> LEVELS = { "debug", "info" };

  time_t t;
  char mbstr[100];

  void formatText( const char* fmt... );
  void setLogLevel( string level );
  void setTime();

public:
  Logger();
  ~Logger(){};

  template<typename T, typename... P>
  void info( T&& format, P&&... params )
  {
    log.clear();
    setTime();
    cout << GREEN << infoHeader << RESET;
    formatText( std::forward<T>( format ), std::forward<P>( params )... );
  };

  template<typename T, typename... P>
  void debug( T&& format, P&&... params )
  {
    if ( logLevel == "debug" ) {
      log.clear();
      setTime();
      cout << YELLOW << debugHeader << RESET;
      formatText( std::forward<T>( format ), std::forward<P>( params )... );
    }
  };

  template<typename T, typename... P>
  void error( T&& format, P&&... params )
  {
    log.clear();
    setTime();
    cout << RED << errorHeader << RESET;
    formatText( std::forward<T>( format ), std::forward<P>( params )... );
  };
};

#endif
