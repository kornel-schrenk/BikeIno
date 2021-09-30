#ifndef TIMEUTILS_H_
#define TIMEUTILS_H_

#include "Arduino.h"
#include "Preferences.h"

#include "M5ez.h"
#include "ezTime.h"

class TimeUtils
{
	public:		
		bool updateTimeFromNTPServer();
		String setDateTime(int year, int month, int day, int hour, int minute, int second);

	private:
		String _getTimezoneLocation();		
};

#endif /* TIMEUTILS_H_ */