#include "libJPLogger.hpp"
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <iomanip>

using namespace std;
using namespace jpCppLibs;


template <typename K, typename V, class C, class A>
std::ostream &operator<< (std::ostream &os, std::map<K,V,C,A> const& m)
{
	os << "{ ";
	typename std::map<K,V,C,A>::const_iterator p;
	for (p = m.begin(); p != m.end(); ++p) {
		os << p->first << ":" << p->second << ", ";
	}
	return os << "}";
}

LoggerExpFileError::LoggerExpFileError(const char* error, bool showErrno ) throw(){
	setMsg(error);
	this->showErrno = showErrno;
}
LoggerExpFileError::LoggerExpFileError(std::string error, bool showErrno ) throw(){
	setMsg(error.c_str());
	this->showErrno = showErrno;
}
LoggerExpFileError::LoggerExpFileError( bool showErrno ) throw(){
	setMsg("Error happened on logger file");
	this->showErrno = showErrno;
}
void
LoggerExpFileError::setMsg(const char * error) throw(){
	myerror = error;
	if( showErrno && errno != 0 ){
		myerror += "[" ;
		myerror += strerror(errno);
		myerror += "]";
	}
}
LoggerExpFileError::~LoggerExpFileError() throw(){
}

const char *
LoggerExpFileError::what() const throw(){
	return myerror.c_str();
}




Logger::Logger( std::string filename )
{
	load();
	if( 0 != setFile( filename ) ){
		return;
	}
}
Logger::Logger()
{
	load();
}
Logger::Logger(Logger * logger)
{
	load();
	copyLoggerDef( logger );
}

void
Logger::load(){
	CONST_DEFMODULE = "ALL";
#ifndef DEBUG
	setLogLvl( CONST_DEFMODULE, M_LOG_MIN, M_LOG_ALLLVL );
#else
	setLogLvl( CONST_DEFMODULE, M_LOG_MAX, M_LOG_ALLLVL );
#endif

	M_LOG_TRANSLATE[M_LOG_TRC] = "TRC";
	M_LOG_TRANSLATE[M_LOG_DBG] = "DBG";
	M_LOG_TRANSLATE[M_LOG_INF] = "INF";
	M_LOG_TRANSLATE[M_LOG_WRN] = "WRN";
	M_LOG_TRANSLATE[M_LOG_ERR] = "ERR";

}

Logger::~Logger(){
	myfile.close();
}

int
Logger::setFile(std::string filename ){
	debugFun( "change filename["<<filename.c_str()<<"]\n");
	if(myfile.is_open()){
		myfile.close();
	}
	myfile.open( filename.c_str() , ios::app );
	if( !myfile.is_open() ){
		cerr << "Log file:[" << filename <<
			"] could not be opened" << endl;
		throw LoggerExpFileError(true);
	}
	outputFile = filename;
	return 0;
}
/**
 * Retrive the file to write the log to
 * @return the file path and name
 */
std::string
Logger::getFile(){
	return outputFile;
}


int
Logger::setLogLvl( std::string module , int logsev, int type)
{
	LogModules::iterator it;
	LogType aux;
	int actType, actlogsev;
	pair<LogType::iterator,bool> ret;
	if( type >= M_LOG_LASTTYPE )
		actType = M_LOG_LASTTYPE - 1;
	else if( type <= M_LOG_NULLTYPE)
		actType = M_LOG_NULLTYPE + 1;
	else
		actType = type;
	if( logsev >= M_LOG_MAX )
		actlogsev = M_LOG_MAX - 1;
	else if( logsev <= M_LOG_NULL )
		actlogsev = M_LOG_NULL + 1;
	else
		actlogsev = logsev;

	aux.insert( pair<int,int>(actType,actlogsev));

	it = Logger::logLvls.find( module );

	if(  Logger::logLvls.end() == it  ){
		Logger::logLvls.insert( pair<std::string,LogType> (module,aux ) );
	}else{
		ret = Logger::logLvls[module].insert(make_pair ( actType , actlogsev ));
		if( false == ret.second )
		{
			 Logger::logLvls[module][actType] = actlogsev;
		}
	}

	//cout << logLvls << endl;

	return 0;
}
int
Logger::unsetModule( std::string module ){
	Logger::logLvls.erase( module );
	return 0;
}

/**
 * Retrieve the log levels and modules of a logger
 * @return Type
 */
const LogModules
Logger::getLogLvls(){
	return (const LogModules )Logger::logLvls;
}


void Logger::log( std::string message , std::string module , int logsev, int type)
{
	LogModules::iterator it;
	LogType::iterator it1;

	try{
		if( writable(module , logsev, type ) )
			write( message , module , type );
	}catch( LoggerExpFileError &e ){
		cerr << e.what();
	}
}
void Logger::log( std::string module , int logsev, int type, std::string message ,...)
{
	va_list args;
	char outMsg[5000];
	std::string toWrite;
	va_start( args, message );
	vsnprintf( outMsg , 5000, message.c_str() , args );
	va_end( args );
	message = outMsg;
	try{
		if( writable(module , logsev, type ) )
			write( message , module , type );
	}catch( LoggerExpFileError &e ){
			cerr << e.what();
	}
}

bool Logger::writable( std::string module , int logsev, int type )
{
	LogModules::iterator it;
	LogType::iterator it1,itl1;

	bool writable = true;
	int actType;
	it = logLvls.find(module);

	if( type > M_LOG_LASTTYPE )
		actType = M_LOG_LASTTYPE - 1;
	else if( type <= M_LOG_NULLTYPE )
		actType = M_LOG_NULLTYPE + 1;
	else
		actType = type;

	//No specific configuration for the module
	// Assumption: M_LOG_ALL_LVL is always set'ed with the log level by default
	if( logLvls.end() == it ){
		//cout << "No cfg for module" << endl;
		//cout <<"No cfg for module:"<< logLvls[CONST_DEFMODULE][M_LOG_ALLLVL] << ">" <<logsev<< endl;
		if(  logLvls[CONST_DEFMODULE][M_LOG_ALLLVL] > logsev )
			writable = false;
	}else{
		it1 = logLvls[module].find(actType);
		//There is no configuration for the log level
		if( logLvls[module].end() == it1 )
		{
			//cout << "not cfg for log level" << endl;
			itl1 = logLvls[module].find(M_LOG_ALLLVL);
			// No configuration in this module for ALL levels
			if( logLvls[module].end() == itl1 ){
				//Assumption M_LOG_ALLLVL is always set'ed with the log level by default
				//cout <<"Mod exist but no cfg for lvl, getting default"<< logLvls[CONST_DEFMODULE][M_LOG_ALLLVL] << ">" <<logsev<< endl;
				if( logLvls[CONST_DEFMODULE][M_LOG_ALLLVL] > logsev )
						writable = false;
			// Configuration for all levels found
			}else if( logLvls[module][M_LOG_ALLLVL] > logsev )
				writable = false;
			//cout <<"Mod exist but no cfg for lvl, getting default of module"<< logLvls[module][M_LOG_ALLLVL] << ">" <<logsev<< endl;
		}else//There is a configuration for the log level
		{
			//cout <<"Has cfg for mode/type:"<< logLvls[module][actType] << ">" <<logsev<< endl;
			if( logLvls[module][actType] > logsev )
			{
				//cout << "actlvl < logsev" << endl;
				writable = false;
			}
		}

	}
	//cout << (writable?"writing":"Not writing") << endl;
	return writable;
}
int Logger::write( std::string message, std::string module , int type ){
	std::lock_guard<std::mutex> lock(mutex);
	debugFun( "writing:[" << module << "][" << type <<  "]" << message<<endl);
	char dateResult[20];
	{
		struct tm *tmp;
		time_t t = time(NULL);

		tmp = localtime(&t);
		if (strftime(dateResult, sizeof(dateResult), "%Y-%m-%d %H:%M:%S", tmp) == 0) {
			throw LoggerExpFileError("Error writing log",true);
		}
	}
	myfile.setf(ios::left);
	myfile << dateResult << " "<< setw(6) << module << "[" << M_LOG_TRANSLATE[type] << "]" <<"\t"
			<< message << endl;
	myfile.flush();


	return 0;
}
/**
 * Set logger level
 * @param lvls Log module levels
 * @return Return 0 in case of success
 */
int
Logger::setLoggerLevel( const LogModules lvls){
	std::lock_guard<std::mutex> lock(mutex);
	debugFun( "Set new log level");
	Logger::logLvls.clear();
	Logger::logLvls = (LogModules)lvls;
}

/**
 * Copy the definitions of a logger
 * @param logger Logger to copy definitions from
 * @return Return 0 in case of success
 */
int
Logger::copyLoggerDef( Logger * logger ){
	//cout << "copyLoggerDef"<<endl;
	debugFun( "Copying the logger[");
	try{
		setFile(logger->getFile());
	}catch( LoggerExpFileError &e ){
		cerr << e.what()<< endl;
	}
	//cout << "going out"<<endl;
	return setLoggerLevel( logger->getLogLvls() );

}

std::unique_ptr<Logger> OneInstanceLogger::inst = nullptr;
std::mutex OneInstanceLogger::m_mutex;
Logger &
OneInstanceLogger::instance(){
	if( nullptr == inst ){
		std::lock_guard<std::mutex> lock(m_mutex);
		if( nullptr == inst )
			inst.reset(new Logger());
	}

	return *inst.get();
};

