#ifndef TIMEUTILS_H_
#define TIMEUTILS_H_

#include "Arduino.h"
#include "Preferences.h"

#include "M5ez.h"
#include "ezTime.h"

class TimeUtils
{
	public:		
		static bool updateTimeFromNTPServer();
		static String createUTCTime(int year, int month, int day, int hour, int minute, int second);

	private:
		static String _getTimezoneLocation();	
		static String _zeroPad(int number);	
};

#endif /* TIMEUTILS_H_ */