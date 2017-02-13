#ifndef QMPCOREPUBLIC_HPP
#define QMPCOREPUBLIC_HPP
#include <cstdint>
#include <vector>
#include <string>
#ifdef _WIN32
#define EXPORTSYM __declspec(dllexport)
#else
#define EXPORTSYM __attribute__ ((visibility ("default")))
#endif
#define QMP_PLUGIN_API_REV "1+indev3"
//MIDI Event structure
struct SEvent
{
	uint32_t iid,time,p1,p2;
	uint8_t type;
	std::string str;
	SEvent(){time=p1=p2=0;type=0;str="";}
	SEvent(uint32_t _iid,uint32_t _t,char _tp,uint32_t _p1,uint32_t _p2,const char* s=NULL)
	{
		iid=_iid;time=_t;type=_tp;
		p1=_p1;p2=_p2;
		if(s)str=std::string(s);else str="";
	}
	friend bool operator <(const SEvent& a,const SEvent& b){return a.time-b.time?a.time<b.time:a.iid<b.iid;}
};
//This struct is used by event reader callbacks and event handler callbacks
//as caller data struct
class SEventCallBackData
{
public:
	uint32_t time,type,p1,p2;
	SEventCallBackData(uint32_t _t,uint32_t _p1,uint32_t _p2,uint32_t _tm){type=_t;p1=_p1;p2=_p2;time=_tm;}
};
//MIDI File class
class CMidiFile{
	public:
		bool valid;
		char *title,*copyright;
		std::vector<SEvent> eventList;
		uint32_t std,divs;
};
//Generic callback function that can be used for hooking the core.
//"userdata" is set when you register the callback function.
class IMidiCallBack
{
	public:
		IMidiCallBack(){}
		virtual void callBack(void* callerdata,void* userdata)=0;
		virtual ~IMidiCallBack(){}
};
//MIDI file reader interface. Use this to implement your file importer.
class IMidiFileReader
{
	public:
		IMidiFileReader(){}
		virtual ~IMidiFileReader(){}
		virtual CMidiFile* readFile(const char* fn)=0;
		virtual void discardCurrentEvent()=0;
		virtual void commitEventChange(SEventCallBackData d)=0;
};
//Main plugin interface.
class qmpPluginIntf
{
	public:
		qmpPluginIntf(){}
		virtual ~qmpPluginIntf(){}
		virtual void init(){}
		virtual void deinit(){}
		virtual const char* pluginGetName(){return "";}
		virtual const char* pluginGetVersion(){return "";}
};
class qmpFuncBaseIntf
{
	public:
		qmpFuncBaseIntf(){}
		virtual void show()=0;
		virtual void close()=0;
		virtual ~qmpFuncBaseIntf(){}
};
//Visualization plugin pinterface. If your plugin implements a visualization,
//you should implement this pinterface.
class qmpVisualizationIntf:public qmpFuncBaseIntf
{
	public:
		qmpVisualizationIntf(){}
		virtual void show()=0;
		virtual void close()=0;
		virtual void start()=0;
		virtual void stop()=0;
		virtual void pause()=0;
		virtual void reset()=0;
		virtual ~qmpVisualizationIntf(){}
};
#ifdef QMP_MAIN
extern "C"{
#endif
//The API class provided by the core. Plugins use this class to interact with
//the core.
class qmpPluginAPI
{
	public:
		virtual ~qmpPluginAPI();
		virtual uint32_t getDivision();
		virtual uint32_t getRawTempo();
		virtual double getRealTempo();
		virtual uint32_t getTimeSig();
		virtual int getKeySig();
		virtual uint32_t getNoteCount();
		virtual uint32_t getMaxTick();
		virtual uint32_t getCurrentPolyphone();
		virtual uint32_t getMaxPolyphone();
		virtual uint32_t getCurrentTimeStamp();
		virtual uint32_t getCurrentPlaybackPercentage();
		virtual int getChannelCC(int ch,int cc);
		virtual int getChannelPreset(int ch);
		virtual void playerSeek(uint32_t percentage);
		virtual double getPitchBend(int ch);
		virtual bool getChannelMask(int ch);
		virtual std::string getTitle();
		virtual std::wstring getWTitle();
		virtual std::string getChannelPresetString(int ch);
		virtual bool isDarkTheme();

		//WARNING!!: This function should be called from event reader callbacks only and
		//it is somehow dangerous -- other plugins might be unaware of the removal of the
		//event. The design might be modified afterward.
		virtual void discardCurrentEvent();
		//WARNING!!: This function should be called from event reader callbacks only and
		//it is somehow dangerous -- other plugins might be unaware of the event change.
		//The design might be modified afterward.
		virtual void commitEventChange(SEventCallBackData d);
		//This function should be called from a file reader when it has read a new event
		virtual void callEventReaderCB(SEventCallBackData d);
		virtual void setFuncState(std::string name,bool state);
		virtual void setFuncEnabled(std::string name,bool enable);

		virtual void registerFunctionality(qmpFuncBaseIntf* i,std::string name,std::string desc,const char* icon,int iconlen,bool checkable);
		virtual void unregisterFunctionality(std::string name);
		virtual void registerVisualizationIntf(qmpVisualizationIntf* intf,std::string name,std::string desc,const char* icon,int iconlen);
		virtual void unregisterVisualizationIntf(std::string name);
		virtual int registerEventReaderIntf(IMidiCallBack* cb,void* userdata);
		virtual void unregisterEventReaderIntf(int intfhandle);
		virtual int registerEventHandlerIntf(IMidiCallBack* cb,void* userdata);
		virtual void unregisterEventHandlerIntf(int intfhandle);
		virtual int registerFileReadFinishedHandlerIntf(IMidiCallBack* cb,void* userdata);
		virtual void unregisterFileReadFinishedHandlerIntf(int intfhandle);
		virtual void registerFileReader(IMidiFileReader* reader,std::string name);
		virtual void unregisterFileReader(std::string name);

		//if desc=="", the option won't be visible in the settings form.
		//it will only show up in the configuration file.
		virtual void registerOptionInt(std::string tab,std::string desc,std::string key,int min,int max,int defaultval);
		virtual int getOptionInt(std::string key);
		virtual void setOptionInt(std::string key,int val);
		virtual void registerOptionUint(std::string tab,std::string desc,std::string key,unsigned min,unsigned max,unsigned defaultval);
		virtual unsigned getOptionUint(std::string key);
		virtual void setOptionUint(std::string key,unsigned val);
		virtual void registerOptionBool(std::string tab,std::string desc,std::string key,bool defaultval);
		virtual bool getOptionBool(std::string key);
		virtual void setOptionBool(std::string key,bool val);
		virtual void registerOptionDouble(std::string tab,std::string desc,std::string key,double min,double max,double defaultval);
		virtual double getOptionDouble(std::string key);
		virtual void setOptionDouble(std::string key,double val);
		virtual void registerOptionString(std::string tab,std::string desc,std::string key,std::string defaultval,bool ispath=false);
		virtual std::string getOptionString(std::string key);
		virtual void setOptionString(std::string key,std::string val);
		virtual void registerOptionEnumInt(std::string tab,std::string desc,std::string key,std::vector<std::string> options,int defaultval);
		virtual int getOptionEnumInt(std::string key);
		virtual void setOptionEnumInt(std::string key,int val);
};
#ifdef QMP_MAIN
}
#endif
//The entry type for the plugin. Your plugin should implement
//qmpPluginIntf* qmpPluginGetInterface(qmpPluginAPI* api)
//as its entry point. A pointer to the core API is also passed to the plugin
//through the parameter. This function should return a pointer to a class
//that implementes the plugin pinterface (qmpPluginIntf).
typedef qmpPluginIntf*(*qmpPluginEntry)(qmpPluginAPI*);
//The following symbol only presents in plugins. Its purpose is to help the core reject incompatible plugins.
typedef const char*(*qmpPluginAPIRevEntry)();
#endif // QMPCOREPUBLIC_HPP
