#include "include/Logger.h"

Logger::Logger()
{
  log.reserve( 2048 );

  const char* env = getenv( ZEZLUP_LOG_LEVEL );
  if ( env )
    setLogLevel( string( env ) );
  else
    logLevel = "info";
};

void
Logger::setLogLevel( string _level )
{
  auto level = find( levels.begin(), levels.end(), _level );
  logLevel = level != levels.end() ? *level : "info";
};

void
Logger::setTime()
{
  t = time( nullptr );
  if ( strftime( mbstr, sizeof( mbstr ), "%c", localtime( &t ) ) )
    cout << BLUE << '[' << mbstr << "] " << RESET;
};

void
Logger::formatText( const char* fmt... )
{
  s32 integer;
  const char* str;

  va_list args;
  va_start( args, fmt );

  while ( *fmt != '\0' ) {
    if ( *fmt == 'd' ) {
      integer = va_arg( args, int );

      log += std::to_string( integer );
    } else if ( *fmt == 'c' ) {
      integer = va_arg( args, int );

      log += std::to_string( integer );
    } else if ( *fmt == 'f' ) {
      doubleStream.precision( 2 );
      doubleStream << fixed;
      doubleStream << va_arg( args, double );

      log += doubleStream.str();
    } else if ( *fmt == 's' ) {
      str = va_arg( args, const char* );

      log += str;
    }

    ++fmt;
  }

  va_end( args );
  cout << log << '\n';
};
