#include "Parser.h"

Parser::Parser( s8 _sectionMarker, s8 _commentMarker )
  : sectionMarker( _sectionMarker )
  , commentMarker( _commentMarker ){};

void
Parser::loadFile( const char* fileName )
{
  ifstream fileHandler;
  stringstream buff;

  fileHandler.open( fileName );
  if ( !fileHandler.is_open() ) {
    logger.error( "ss", "There was an error opening ", fileName );
    throw 1;
  }
  logger.info( "ss", "Loading file ", fileName );
  buff << fileHandler.rdbuf();
  fileBuffer = buff.str();

  fileHandler.close();
};

void
Parser::parse()
{
  size_t pos = 0, nextSectionMarker;

  while ( pos != string::npos ) {
    pos = fileBuffer.find( sectionMarker, pos );

    if ( pos != string::npos ) {
      nextSectionMarker = fileBuffer.find( sectionMarker, pos + 1 );

      parseSection( pos, nextSectionMarker );
      pos = nextSectionMarker;
    }
  }
};

void
Parser::parseSection( const size_t start, const size_t end )
{
  size_t endParagr, position = start;

  endParagr = fileBuffer.find( NEXT_LINE, position );
  string sectionName = fileBuffer.substr( position, endParagr - position );
  trim( sectionName );
  logger.info( "ss", "Found: ", sectionName.c_str() );
  sections.push_back( sectionName );
  mappedSections.insert( { sectionName, vector<string>() } );
  position = endParagr + 1;

  while ( position != end && position >= start ) {
    if ( fileBuffer[position] == NEXT_LINE ) {
      ++position;

    } else if ( fileBuffer[position] == commentMarker ) {
      position = fileBuffer.find( NEXT_LINE, position ) + 1;

    } else {
      endParagr = fileBuffer.find( NEXT_LINE, position );
      mappedSections[sectionName].push_back(
        fileBuffer.substr( position, endParagr - position ) );

      position = endParagr + 1;
    }
  }
};

vector<string>
Parser::getSectionData( const string& s )
{
  try {
    return mappedSections.at( s );
  } catch ( const out_of_range& oor ) {
    logger.error( "s", "Error accessing category" );
    return vector<string>();
  }
};
