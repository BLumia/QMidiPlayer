#include <cstdio>
#include "bongocatplugin.hpp"
#include "bongocatwindow.hpp"

qmpBongoCatPlugin::qmpBongoCatPlugin(qmpPluginAPI* _api)
{
    api=_api;

}

qmpBongoCatPlugin::~qmpBongoCatPlugin()
{
    api=nullptr;
}

void qmpBongoCatPlugin::init()
{
    fputs("Hello world from plugin init!\n",stderr);
    form = new BongoCatWindow(nullptr);
    api->registerEventHandlerIntf(form, nullptr);

    form->show();
}

void qmpBongoCatPlugin::deinit()
{
    fputs("Bye!\n",stderr);
    form->deleteLater();
}

const char* qmpBongoCatPlugin::pluginGetName()
{
    return "Cutie Bongo Cat Plugin";
}

const char* qmpBongoCatPlugin::pluginGetVersion()
{
    return "0.0.1";
}
