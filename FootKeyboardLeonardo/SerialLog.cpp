#ifndef SerialLog_CPP
#define SerialLog_CPP
#include "Arduino.h"
#include "String.h"
#include "SerialLog.h"



  static const void SetSerialLog(bool setLogOn){ m_useLog = setLogOn; }
  static const void LogPrint() {
	 if (m_useLog)
		 Serial.print(m_temp);
 }
 static const void LogPrintLn() {

	 if (m_useLog)
		 Serial.println(m_temp);
 }
 static const void LogPrint(String text) {
	 m_temp = text;
	 LogPrint();
 }
 static const void LogPrintLn(String text) {
	 m_temp = text;
	 LogPrintLn();
 }
 static const void LogPrintDoubleLn(String text, String text2) {
	 m_temp = text;
	 LogPrint();
	 m_temp = text2;
	 LogPrintLn();
 }


 static const void LogPrint(bool value) {
	 m_temp = value;
	 LogPrint();
 }
 static const void LogPrintLn(bool value) {
	 m_temp = value;
	 LogPrintLn();

 } 
 static const void LogPrint(int value) {
	 m_temp = value;
	 LogPrint();
 }
 static const void LogPrintLn(int value) {
	 m_temp = value;
	 LogPrintLn();
 }

#endif