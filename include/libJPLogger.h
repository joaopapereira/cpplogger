/**
 *  Copyright 2012 Joao Pereira<joaopapereira@gmail.com>
 *
 *
 *  This file is part of libJPLogger.
 *
 *  libJPSemaphores is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libJPSemaphores is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libJPLogger.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef libJPLogger_H
#define libJPLogger_H

#include <iostream>
#include <fstream>
#include <map>
#include <time.h>
#include <exception>

/**
 * This part is used to ensure more debug
 */
#ifdef DEBUG
	#define debugFun( __str ) \
		do{ \
			cerr << __FUNCTION__  << " " << __str; \
		}while(0);
	#define debug( __str ) \
		do{ \
			cerr << __str; \
		}while(0);
#else
	#define debugFun( __str )  \
	do { \
	}while(0);
	#define debug( __str )  \
	do { \
	}while(0);

#endif

/**
 * This enum have the log levels available
 */
enum{
	M_LOG_NULL,
	M_LOG_MIN,
	M_LOG_LOW,
	M_LOG_NRM,
	M_LOG_HGH,
	M_LOG_MAX,
	M_LOG_NO

};

/**
 * This enum have the log types available
 */
enum{
	M_LOG_NULLTYPE,
	M_LOG_TRC,
	M_LOG_DBG,
	M_LOG_INF,
	M_LOG_WRN,
	M_LOG_ERR,
	M_LOG_ALLLVL,
	M_LOG_LASTTYPE
};

/**
 * Class the implements the exceptions of the logger
 */
class LoggerExpFileError: public std::exception{
public:
	/**
	 * Constructor used when no message needs to be passed
	 * @param showErrno Indicates if the ERRNO should be or not written
	 */
	LoggerExpFileError(bool showErrno = false) throw();
	/**
	 * Constructor used when no message needs to be passed
	 * @param error Error message
	 * @param showErrno Indicates if the ERRNO should be or not written
	 */
	LoggerExpFileError( char* error,bool showErrno = false ) throw();
	/**
	 * Constructor used when no message needs to be passed
	 * @param error Error message
	 * @param showErrno Indicates if the ERRNO should be or not written
	 */
	LoggerExpFileError(std::string error, bool showErrno = false) throw();
	/**
	 * Function to change error message
	 * @param error Error message
	 */
	void setMsg(const char * error) throw();
	/**
	 * Retrive error reason
	 * @return String with the error
	 */
	const char * what() const throw();
	/**
	 * Class destructor
	 */
	~LoggerExpFileError() throw();
private:
	/**
	 * Error Message
	 */
	std::string myerror;
	/**
	 * Indicates if errno should be shown or not
	 */
	bool showErrno;
};

/**
 * This type contains the correlation between
 * log type and log severity
 */
typedef std::map<int,int> LogType;
/**
 * This type contains the correlation between
 * the module and the log type and log severity
 */
typedef std::map<std::string,LogType> LogModules;

/**
 * Class logger
 */
class Logger{
	public:
		/**
		 * Class constructor that receives another logger
		 */
		Logger( Logger* useLogger );
		/**
		 * Class constructor that receives the file to right in
		 */
		Logger( std::string filename);
		/**
		 * Class destructor
		 */
		~Logger();

		/**
		 * Class constructor without arguments
		 */
		Logger();

		/**
		 * Change the filename to write to
		 * @param filename File path and name
		 * @return Return 0 in case of success
		 */
		int setFile(std::string filename );
		/**
		 * Retrive the file to write the log to
		 * @return the file path and name
		 */
		std::string getFile();

		/**
		 * Writes the log
		 * @param message Message to be written
		 * @param module Module that whats the message written
		 * @param logsev Log severity
		 * @param type Type of the log
		 */
		void log(std::string message , std::string module , int logsev, int type);
		/**
		 * Writes the log
		 * @param module Module that whats the message written
		 * @param logsev Log severity
		 * @param type Type of the log
		 * @param format Format of the message to be written
		 * @param ... The function accept multiple parameters to add to format
		 */
		void log(std::string module , int logsev, int type,std::string format , ... );

		/**
		 * Change a log level of a module
		 * @param module Name of the module
		 * @param logsev The minimum level of severity that will be shown
		 * @param type Type of the log
		 */
		int setLogLvl( std::string module , int logsev, int type);
		/**
		 * Remove configuration of a module
		 * @param module Name of the module
		 * @return Returns 0 in case of success
		 */
		int unsetModule( std::string module );

		/**
		 * Retrieve the log levels and modules of a logger
		 * @return Type
		 */
		const LogModules getLogLvls();

		/**
		 * Copy the definitions of a logger
		 * @param logger Logger to copy definitions from
		 * @return Return 0 in case of success
		 */
		int copyLoggerDef( Logger * logger );

	private:
		/**
		 * Map between the modules and the types/levels
		 */
		LogModules logLvls;
		/**
		 * Output file
		 */
		std::ofstream myfile;
		/**
		 * Default module
		 */
		std::string CONST_DEFMODULE;
		/**
		 * File to output logs to
		 */
		std::string outputFile;

		/**
		 * Check if it is possible to write
		 * @param module Name of the module
		 * @param loglevel Level of the log
		 * @param type Type of the log
		 * @return True if can write log.
		 */
		bool writable( std::string module , int loglevel, int type );
		/**
		 * Writes the log
		 * @param message Message to be written
		 * @param module Module that whats the message written
		 * @param type Type of the log
		 */
		int write( std::string message, std::string module , int type );

		/**
		 * Set logger level
		 * @param lvls Log module levels
		 * @return Return 0 in case of success
		 */
		int setLoggerLevel( const LogModules lvls);

		/**
		 * Map of log type
		 */
		std::string M_LOG_TRANSLATE[M_LOG_LASTTYPE];

		/**
		 * load configuration to the the logger
		 */
		void load();
};

/**
 * This class implements a Singleton to the logger
 * This class should be used if you need only one
 * instance of the logger for all the application
 */
class OneInstanceLogger: public Logger{
public:
	/**
	 * Get and instance of the logger
	 * @return An instance to the logger
	 */
	static OneInstanceLogger *instance();

protected:
	/**
	 * Class constructor
	 */
	OneInstanceLogger();
	/**
	 * Logger instance
	 */
	static OneInstanceLogger *inst;
};

#endif
