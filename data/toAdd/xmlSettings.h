#include <iostream>
#include "string.h"
#include "tinyxml.h"

using namespace std;

#define MAX_TAG_VALUE_LENGTH_IN_CHARS		1024

class xmlSettings{

public:
	xmlSettings();
	xmlSettings(const string& xmlFile);

	~xmlSettings();

	void setVerbose(bool _verbose);

	bool loadFile(const string& xmlFile);
	bool saveFile(const string& xmlFile);
	bool saveFile();


	int 	getValue(const string&  tag, int            defaultValue, int which = 0);
	double 	getValue(const string&  tag, double         defaultValue, int which = 0);
	string 	getValue(const string&  tag, const string& 	defaultValue, int which = 0);

	int 	setValue(const string&  tag, int            value, int which = 0);
	int 	setValue(const string&  tag, double         value, int which = 0);
	int 	setValue(const string&  tag, const string& 	value, int which = 0);

	TiXmlDocument	doc;
	bool			bDocLoaded;

protected:
	TiXmlHandle		storedHandle;
	int level;

	bool 	readTag(const string&  tag, TiXmlHandle& valHandle, int which = 0);	// max 1024 chars...
	int 	writeTag(const string&  tag, const string& valueString, int which = 0);
};