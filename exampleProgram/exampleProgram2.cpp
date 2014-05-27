/*
 ============================================================================
 Name        : exampleProgram2.c
 Author      : Joao Pereira
 Version     :
 Copyright   : This library is creating under the MIT license
 Description : Uses shared library to print greeting
               To run the resulting executable the LD_LIBRARY_PATH must be
               set to ${project_loc}/libJPLogger/.libs
               Alternatively, libtool creates a wrapper shell script in the
               build directory of this program which can be used to run it.
               Here the script will be called exampleProgram.
 ============================================================================
 */
#define DEBUG 1
#include "libJPLogger.hpp"

using namespace jpCppLibs;
// With this option will disable the debug to screen and do not allow any default log to be written

int main(void) {
  Logger log("/tmp/test.log");
  log.setLogLvl("ALL",M_LOG_MIN,M_LOG_ALLLVL);

  //auto infLogger = log.log("MOD1", M_LOG_NRM, M_LOG_INF);
  log.log("MOD1", M_LOG_NRM, M_LOG_INF) << "Testing new functionality" << std::endl;
  log.log("MOD1", M_LOG_NRM, M_LOG_INF) << "Testing new functionality" << std::endl;

  {
	  log.log("MOD1", M_LOG_NRM, M_LOG_INF) << "let's see if i die!!" << std::endl;
  }

  log.setLogLvl("Ex1Set1",M_LOG_MAX,M_LOG_WRN);
  log.log("Ex1Set1",M_LOG_LOW,M_LOG_WRN) << "I shall not appear" << std::endl;


  return 0;
}
