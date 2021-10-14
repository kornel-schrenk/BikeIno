#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <Arduino.h>
#include <SD.h>

class FileUtils
{
	public:		
        void openRideLog(String filePath);
        void appendRideLog(String filePath, String message);

	private:
};

#endif /* FILEUTILS_H_ */