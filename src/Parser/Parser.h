#ifndef PARSER_H
#define PARSER_H

#include "../Logger/Logger.h"
#include "../types.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Parser
{
  const s8 NEXT_LINE = '\n';
  string fileBuffer;

  unordered_map<string, vector<string>> mappedSections;

  vector<string> sections;
  s8 sectionMarker;
  s8 commentMarker;

  Logger logger;

public:
  Parser() = delete;
  Parser( s8 _sectionMarker, s8 _commentMarker );

  void parse();
  void parseSection( const size_t start, const size_t end );
  void loadFile( const char* fileName );
  vector<string> getSectionData( const string& s );

  inline void friend trim( string& s );
};

inline void
ltrim( string& s )
{
  s.erase( s.begin() );
  s.erase( s.begin(), find_if( s.begin(), s.end(), []( unsigned char ch ) {
             return !isspace( ch );
           } ) );
};

inline void
rtrim( string& s )
{
  s.erase( find_if( s.rbegin(),
                    s.rend(),
                    []( unsigned char ch ) { return !isspace( ch ); } )
             .base(),
           s.end() );
};

inline void
trim( string& s )
{
  ltrim( s );
  rtrim( s );
};

#endif
