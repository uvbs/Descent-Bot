/****************************************************
 * This work is licensed under the Creative
 * Commons Attribution-NonCommercial-ShareAlike
 * 3.0 Unported License. To view a copy of this
 * license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 * or send a letter to Creative Commons, 444
 * Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 ***************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>
#include <map>

using namespace std;

#include "xml.h"
#include "log.h"
#include "log_driver_raw.h"
#include "log_driver_file.h"

class CConfig;
class CConfig_Logging;

class CConfig_Logging_Logger
{
public:
	string get_Name() const;
	LogType get_Type() const;
	int get_Level() const;
	string get_Driver() const;
	string operator[](const string &key);
	vector<string> get_Option_Names();
protected:
private:
	string mName;
	LogType mType;
	int mLevel;
	string mDriver;
	map<string, string> mOptions;

	CConfig_Logging_Logger();
	CConfig_Logging_Logger(const CConfig_Logging_Logger &source);
	CConfig_Logging_Logger(CXMLNode &node);
	~CConfig_Logging_Logger();
	CConfig_Logging_Logger &operator=(const CConfig_Logging_Logger &source);

	bool is_Valid() const;

	friend class CConfig_Logging;
};


class CConfig_Logging
{
public:
	CConfig_Logging_Logger &operator[](const string &index) const;
	vector<string> get_Names() const;
protected:
private:
	map<string, CConfig_Logging_Logger *> mLoggers;

	CConfig_Logging();
	CConfig_Logging(const CConfig_Logging &source);
	~CConfig_Logging();

	LogType get_LogType(CXMLNode &node);

	CConfig_Logging &operator=(const CConfig_Logging &source);

	bool Load_Logging(CXMLNode &node);

	friend class CConfig;
};

/// This class represents the current configuration setup
class CConfig 
{
public:
	~CConfig();

	static CConfig &getInstance();

	bool Load(const string filename);

	void Reset();

	CConfig_Logging Logging;
protected:
private:
	static CConfig *mSingleton;

	void Initialize();

	static void clearInstance();
	
	friend class CConfig_Logging;

	CConfig();
	CConfig(const CConfig &source);
	CConfig &operator=(const CConfig &source);
};

#endif
