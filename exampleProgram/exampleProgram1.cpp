/*
 ============================================================================
 Name        : exampleProgram.c
 Author      : Joao Pereira
 Version     :
 Copyright   : This library is creating under the GPL license
 Description : Uses shared library to print greeting
               To run the resulting executable the LD_LIBRARY_PATH must be
               set to ${project_loc}/libJPLogger/.libs
               Alternatively, libtool creates a wrapper shell script in the
               build directory of this program which can be used to run it.
               Here the script will be called exampleProgram.
 ============================================================================
 */
#define DEBUG 0
#include "libJPLogger.hpp"

using namespace jpCppLibs;
// With this option will disable the debug to screen and do not allow any default log to be written

int main(void) {
  Logger log("/tmp/test.log");
  log.log("Before first set","Ex1",M_LOG_LOW,M_LOG_INF);
  log.setLogLvl("Ex1",M_LOG_MIN,M_LOG_INF);
  log.log("Before change to normal","Ex1",M_LOG_LOW,M_LOG_INF);
  log.setLogLvl("Ex1",M_LOG_NRM,M_LOG_INF);

  log.log("This log will not appear","Ex1",M_LOG_LOW,M_LOG_INF);
  log.log("But this one will","Ex1",M_LOG_HGH,M_LOG_INF);
  log.log("And this one too","Ex1",M_LOG_NRM,M_LOG_INF);


  OneInstanceLogger::instance()->log("Try to log with high[not show]","Ex1",M_LOG_HGH,M_LOG_INF);

  OneInstanceLogger::instance()->copyLoggerDef( &log );
  OneInstanceLogger::instance()->log("After the copy","Ex1",M_LOG_HGH,M_LOG_INF);
  OneInstanceLogger::instance()->setLogLvl("Ex1",M_LOG_NRM,M_LOG_INF);
  OneInstanceLogger::instance()->log("After copied change","Ex1",M_LOG_LOW,M_LOG_INF);

  log.setLogLvl("Ex1Set1",M_LOG_MIN,M_LOG_WRN);
  log.log("I will appear","Ex1Set1",M_LOG_LOW,M_LOG_WRN);
  OneInstanceLogger::instance()->log("But i will not","Ex1Set1",M_LOG_LOW,M_LOG_WRN);


  return 0;
}
