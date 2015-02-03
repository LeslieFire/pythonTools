#include "xmlSettings.h"

#include <vector>
#include <string>
#include <iostream>
#include "utils.h"


const float floatPrecision = 9;

//----------------------------------------
// a pretty useful tokenization system:
static vector<string> tokenize(const string & str, const string & delim);
static vector<string> tokenize(const string & str, const string & delim)
{
	vector<string>	tokens;
	size_t p0 = 0, p1 = string::npos;

	while (p0 != string::npos){
		p1 = str.find_first_of(delim, p0);
		if (p1 != p0){
			string token = str.substr(p0, p1 - p0);
			tokens.push_back(token);
		}
		p0 = str.find_first_not_of(delim, p1);
	}
	return tokens;
}

xmlSettings::xmlSettings():
	storedHandle(NULL)
{

	level		 = 0;
	storedHandle = TiXmlHandle(&doc);
}

xmlSettings::xmlSettings(const string& xmlFile):
	storedHandle(NULL)	
{
	level			= 0;
	storedHandle	= TiXmlHandle(&doc);
	loadFile(xmlFile);
}

//---------------------------------------------------------
xmlSettings::~xmlSettings()
{
}

void xmlSettings::setVerbose(bool _verbose){

}

bool xmlSettings::loadFile(const string& xmlFile){
	string fullXmlFile = ofToDataPath(xmlFile);
	cout << "Load file from " + fullXmlFile <<endl; 
	bool loadOkay = doc.LoadFile(fullXmlFile);

	level = 0;

	storedHandle = TiXmlHandle(&doc);
	return loadOkay;
}

bool xmlSettings::saveFile(const string& xmlFile){
	string fullXmlFile = ofToDataPath(xmlFile);
	cout << "Save file to " + fullXmlFile <<endl; 
	return doc.SaveFile(fullXmlFile);
}

bool xmlSettings::saveFile(){
	return doc.SaveFile();
}


int xmlSettings::getValue(const string&  tag, int defaultValue, int which){
	TiXmlHandle	valHandle(NULL);
	if (readTag(tag, valHandle, which)){
		cout << "Get " + tag + " " + valHandle.ToText()->Value() << endl;
		return ofToInt(valHandle.ToText()->Value());
	}
	return defaultValue;
}

double xmlSettings::getValue(const string&  tag, double defaultValue, int which){
	TiXmlHandle valHandle(NULL);
	if (readTag(tag, valHandle, which)){
		cout << "Get " + tag + " " + valHandle.ToText()->Value() << endl;
		return ofToFloat(valHandle.ToText()->Value());
	}
	return defaultValue;
}

//---------------------------------------------------------
string xmlSettings::getValue(const string& tag, const string& defaultValue, int which){
	TiXmlHandle valHandle(NULL);
	if (readTag(tag, valHandle, which)){
		cout << "Get " + tag + " " + valHandle.ToText()->Value() << endl;
		return valHandle.ToText()->ValueStr();
	}
	return defaultValue;
}

bool xmlSettings::readTag(const string&  tag, TiXmlHandle& valHandle, int which){
	vector<string> tokens = tokenize(tag, ":");

	TiXmlHandle tagHandle = storedHandle;
	for (int x = 0; x <(int)tokens.size(); ++x){
		(x == 0) ? tagHandle = tagHandle.ChildElement(tokens.at(x),which) : tagHandle = tagHandle.FirstChildElement( tokens.at(x) );
	}

	// once we've walked, let's get that value...
	valHandle = tagHandle.Child(0);
	return (valHandle.ToText() != NULL);
}

int xmlSettings::setValue(const string& tag, int value, int which){
	int tagID = writeTag(tag, ofToString(value).c_str(), which) -1;
	cout << "save " + tag + " " + ofToString(value) << endl;
	return tagID;
}

int xmlSettings::setValue(const string& tag, double value, int which){
	int tagID = writeTag(tag, ofToString(value, floatPrecision).c_str(), which) -1;
	cout << "save " + tag + " " + ofToString(value) << endl;
	return tagID;
}

int xmlSettings::setValue(const string& tag, const string &value, int which){
	int tagID = writeTag(tag, value, which) -1;
	cout << "save " + tag + " " + value << endl;
	return tagID;
}

int xmlSettings::writeTag(const string&  tag, const string& valueStr, int which){
	vector<string> tokens = tokenize(tag, ":");

	// allocate on the stack
	vector <TiXmlElement> elements;
	elements.reserve(tokens.size());
	for (int x = 0; x < (int)tokens.size(); ++x){
		elements.push_back(tokens.at(x));
	}

	TiXmlText Value(valueStr);

	// search our way up - do these tags exist?
	// find the first that DOESNT exist, then move backwacds...
	TiXmlHandle tagHandle = storedHandle;

	bool addNewTag = false;
	if (which == -1) addNewTag = true;

	for (int x = 0; x < (int)tokens.size(); ++x){

		if ( x > 0){
			//multi tags of same name
			//only for the root level
			which = 0;
			addNewTag = false;
		}
		TiXmlHandle isRealHandle = tagHandle.ChildElement( tokens.at(x), which );

		if ( !isRealHandle.ToNode() || addNewTag){

			for ( int i = (int) tokens.size() - 1; i >= x; --i){
				(i == (int)tokens.size()-1) ? elements[i].InsertEndChild(Value) : elements[i].InsertEndChild(elements[i+1]);
			}

			tagHandle.ToNode()->InsertEndChild(elements[x]);
			break;
		} else {
			tagHandle = isRealHandle;
			if ( x == (int)tokens.size() - 1){
				// what we want to change : TiXmlHandle valHandle = tagHandle.ChildElement(0);
				tagHandle.ToNode()->Clear();
				tagHandle.ToNode()->InsertEndChild(Value);
			}
		}
	}
	//lets count how many tags with our name exist so we can return an index

	//ripped from tinyXML as doing this ourselves once is a LOT! faster
	//than having this called n number of times in a while loop - we go from n*n iterations to n iterations
	int numSameTags;
	TiXmlElement *child = ( storedHandle.FirstChildElement( tokens.at(0) ) ).ToElement();
	for (numSameTags = 0; child; child = child->NextSiblingElement( tokens.at(0) ), ++numSameTags){
		//nothing
	}
	return numSameTags;
}

