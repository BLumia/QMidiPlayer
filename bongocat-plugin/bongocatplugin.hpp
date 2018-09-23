#ifndef BONGOCATPLUGIN_H
#define BONGOCATPLUGIN_H

#include "../include/qmpcorepublic.hpp"

#include <QtWidgets/QMainWindow>

class BongoCatWindow;
class qmpBongoCatPlugin:public qmpPluginIntf
{
	public:
        qmpBongoCatPlugin(qmpPluginAPI* _api);
        ~qmpBongoCatPlugin();
		void init();
		void deinit();
		const char* pluginGetName();
		const char* pluginGetVersion();
    private:
        BongoCatWindow* form;
        qmpPluginAPI* api;
};

extern "C"{
	EXPORTSYM qmpPluginIntf* qmpPluginGetInterface(qmpPluginAPI* api)
    {return new qmpBongoCatPlugin(api);}
	EXPORTSYM const char* qmpPluginGetAPIRev()
	{return QMP_PLUGIN_API_REV;}
}

#endif // BONGOCATPLUGIN_H
