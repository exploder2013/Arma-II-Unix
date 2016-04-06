#ifndef HEADER_LOGGER
#define HEADER_LOGGER

#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <vector>
#include <iostream>
#include <ctime>

using namespace std;

typedef void( *inputHandler )( char* input );



class Logger
{
public:
	Logger()
	{
			if( !AllocConsole() )
				return;

			CONSOLE_SCREEN_BUFFER_INFO cInfo;
			GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cInfo );

			cInfo.dwSize.Y = 500;

			SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), cInfo.dwSize );

			HANDLE hCon = GetStdHandle( STD_OUTPUT_HANDLE );
			int hConHandle = _open_osfhandle( (long) hCon, _O_TEXT );

			FILE* fp = _fdopen( hConHandle, "w" );
			*stdout = *fp;
			setvbuf( stdout, NULL, _IONBF, 0 );

			hCon = GetStdHandle( STD_INPUT_HANDLE );
			hConHandle = _open_osfhandle( (long) hCon, _O_TEXT );

			fp = _fdopen( hConHandle, "r" );
			*stdin = *fp;
			setvbuf( stdin, NULL, _IONBF, 0 );

			hCon = GetStdHandle( STD_ERROR_HANDLE );
			hConHandle = _open_osfhandle( (long) hCon, _O_TEXT );

			fp = _fdopen( hConHandle, "w" );
			*stderr = *fp;
			setvbuf( stderr, NULL, _IONBF, 0 );

			ios::sync_with_stdio();

			cout << "Exploder's Console - ALL RIGHTS RESERVED" << endl;
	}

	
	
	void registerInput( LPVOID func )
	{
		if( func )
		{
			registedHandlers.push_back( func );
		}
	}
	void handleInput( )
	{
		std::thread t1( &Logger::listener, this );
		t1.detach();
	}

private:
	void listener()
	{
		char input[ 1000 ];

		while( true )
		{
			memset( input, 0, sizeof( input ) ); // clearing buffer.

			cout << "\n>";

			cin.getline( input, sizeof( input ) );

			for( LPVOID hanlder : registedHandlers )
			{
				inputHandler handle = (inputHandler) hanlder;
				handle( input );
			}

		}
	}

	vector<LPVOID> registedHandlers;
};

class FileLogger{

public:
	FileLogger()
	{ 
		file.open( "log.txt", ios::app );
	}
	~FileLogger(){
		file.close();
	}

	void Log( string text ){
		if( file.is_open() ){

			time_t t			= time( 0 );
			char buffer[ 90 ]	= { 0 };

			struct tm curTime; 
			localtime_s( &curTime, &t );


			strftime( buffer, sizeof( buffer ), "[%d %b][%X]: ", &curTime );

			file << buffer << text.c_str() << endl;
		}
	}
	void clearLog(){
		if( file.is_open() ){
			file.close();
			file.open( "log.txt" );
		}
	}

private:
	ofstream file;
};
#endif