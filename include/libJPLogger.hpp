/**
 *  Copyright 2012 Joao Pereira<joaopapereira@gmail.com>
 *
 *
 *  This file is part of libJPLogger.
 *
 *  libJPSemaphores is free software: you can redistribute it and/or modify
 *  it under the terms of the MIT License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libJPSemaphores is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  MIT License for more details.
 *
 */
#ifndef libJPLogger_H
#define libJPLogger_H

#include <iostream>
#include <fstream>
#include <map>
#include <time.h>
#include <exception>
#include <memory>
#include <iomanip>
#include <sstream>
#ifdef USE_BOOST_INSTEAD_CXX11
#include <boost/thread/mutex.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
namespace std{
    template<class T>
    class lock_guard{
      T locker;
      public:
        lock_guard(T locker): locker(locker){
            locker->lock();
        };
        ~lock_guard(){
            locker->unlock();
        };
    };
    typedef boost::mutex mutex;
};

#else
#include <mutex>
#endif

namespace jpCppLibs{
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
	LoggerExpFileError( const char* error,bool showErrno = false ) throw();
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
class LoggerTemporaryStream;

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
	 * Writes the log
	 * @param module Module that whats the message written
	 * @param logsev Log severity
	 * @param type Type of the log
	 */
#ifdef USE_BOOST_INSTEAD_CXX11
	boost::shared_ptr<LoggerTemporaryStream> log(std::string module , int logsev, int type );
#else
	std::unique_ptr<LoggerTemporaryStream> log(std::string module , int logsev, int type );
#endif
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
protected:
	/**
	 * Mutex to ensure that the class is thread safe
	 */
	std::mutex mutex;
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
	int write( std::string message);
	/**
	 * Writes the log line initial
	 * @param module Module that whats the message written
	 * @param type Type of the log
	 */
	int writeLineStart( std::string module , int type );

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
	friend LoggerTemporaryStream;
};
/**
 * Class used as stream to write to the file
 */
class LoggerTemporaryStream: public std::ostream
{
	/**
	 * Class that holds the buffer
	 */
	class LoggerTemporaryBuffer: public std::stringbuf
	{
		/**
		 * Output buffer
		 */
		std::ostream&   output;
		/**
		 * Module of the log
		 */
		std::string module;
		/**
		 * Type of the log
		 */
		std::string type;
		/**
		 * Mutex to synchronize file writing
		 */
		std::mutex *mutex;
	public:
		/**
		 * Class constructor
		 * @param str Output buffer
		 * @param module Module name
		 * @param type Type of log
		 * @param mutex Mutex to synchronize file writing
		 */
		LoggerTemporaryBuffer(std::ostream& str, std::string module, std::string type, std::mutex *mutex)
		:output(str),
		 module(module),
		 type(type),
		 mutex(mutex){};
		/**
		 * Sync function called when std::endl is passed into the stream
		 */
		virtual int sync ( )
		{
			if(0 == module.compare("-1")){
				str("");
				return 0;
			}
#ifdef USE_BOOST_INSTEAD_CXX11
			std::lock_guard<std::mutex*> lock(mutex);
#else
			std::lock_guard<std::mutex> lock(*mutex);
#endif
			char dateResult[20];
			{
				struct tm *tmp;
				time_t t = time(NULL);

				tmp = localtime(&t);
				if (strftime(dateResult, sizeof(dateResult), "%Y-%m-%d %H:%M:%S", tmp) == 0) {
					throw LoggerExpFileError("Error writing log",true);
				}
			}


			output << dateResult << " "<< std::setw(6) << module << "[" << type << "]" <<"\t";
			output << str();
			str("");
			output.flush();
			return 0;
		}
	};

	/**
	 * Buffer
	 */
	LoggerTemporaryBuffer buffer;
	public:
	/**
	 * Class constructor
	 * @param str Output buffer
	 * @param module Module name
	 * @param type Type of log
	 * @param mutex Mutex to synchronize file writing
	 */
	LoggerTemporaryStream(std::ostream& str, std::string module, std::string type, std::mutex* mutex)
	:std::ostream(&buffer)
	,buffer(str, module, type, mutex){};

	/**
	 * Function used to be able to write any type to the stream
	 * @param os Current Stream
	 * @param val Value to write
	 */
	template<typename T>
#ifdef USE_BOOST_INSTEAD_CXX11
	inline friend boost::shared_ptr<LoggerTemporaryStream> const&operator<<(boost::shared_ptr<LoggerTemporaryStream>const&os, const T&val){
#else
	inline friend std::unique_ptr<LoggerTemporaryStream> const&operator<<(std::unique_ptr<LoggerTemporaryStream>const&os, const T&val){
#endif
		*os << val;
		return os;
	}
	/**
	 * Function used to be able to write from functions or objects like std::endl
	 * @param os Current Stream
	 * @param val Value to write
	 */
#ifdef USE_BOOST_INSTEAD_CXX11
	inline friend boost::shared_ptr<LoggerTemporaryStream> const&operator<<(boost::shared_ptr<LoggerTemporaryStream>const&os, std::ostream&(*f)(std::ostream&) )
#else
	inline friend std::unique_ptr<LoggerTemporaryStream> const&operator<<(std::unique_ptr<LoggerTemporaryStream>const&os, std::ostream&(*f)(std::ostream&) )
#endif
	{
		*os << f;
		return os;
	}
};

/**
 * This class implements a Singleton to the logger
 * This class should be used if you need only one
 * instance of the logger for all the application
 */
class OneInstanceLogger{
public:
	/**
	 * Get and instance of the logger
	 * @return An instance to the logger
	 */
	static Logger &instance();

protected:
	/**
	 * Class constructor
	 */
	OneInstanceLogger();
	/**
	 * Copy constructor
	 */
	OneInstanceLogger( const OneInstanceLogger&other);
	/**
	 * Attribution operator
	 */
	OneInstanceLogger& operator= (const OneInstanceLogger& rs);
	/**
	 * Logger instance
	 */
#ifdef USE_BOOST_INSTEAD_CXX11
	static boost::shared_ptr<Logger> inst;
#else
	static std::unique_ptr<Logger> inst;
#endif
	/**
	 * Mutex to ensure that only 1 instance
	 * exist
	 */
	static std::mutex m_mutex;
};
};

#endif
