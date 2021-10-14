#include "utils/FileUtils.h"

void FileUtils::openRideLog(String filePath)
{        
    File file = SD.open(filePath, FILE_WRITE);
    if (!file) {
        Serial.println(F("Failed to open ride log file for writing"));
        return;
    }
    
    //Write XML file definition into the file
    file.print(F("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));
    file.close();
}
    
void FileUtils::appendRideLog(String filePath, String message)
{   
    File file = SD.open(filePath, FILE_APPEND);
    if (!file) {
        Serial.println(F("Failed to open ride log file for appending"));
        return;
    }
    
    file.print(message);    
    file.close();
}