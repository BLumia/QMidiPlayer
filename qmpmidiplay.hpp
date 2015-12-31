//sorry for the stupid C-like code...
#ifndef QMPMIDIPLAY_H
#define QMPMIDIPLAY_H
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <fluidsynth.h>
struct SEvent
{
	uint32_t iid,time,p1,p2;
	uint8_t type;
	char *str;
	SEvent(){time=p1=p2=0;type=0;str=NULL;}
	SEvent(uint32_t _iid,uint32_t _t,char _tp,uint32_t _p1,uint32_t _p2,const char* s=NULL)
	{
		iid=_iid;time=_t;type=_tp;
		p1=_p1;p2=_p2;
		if(s){str=new char[strlen(s)+2];strcpy(str,s);}else str=NULL;
	}
};
class CMidiFile
{
	private:
		SEvent *eventList[10000000];
		char *title,*copyright;
		uint32_t eventc;
		uint32_t fmt,trk,divs;
		FILE *f;
		int byteread;
		uint32_t notes,curt,curid;

		void error(int fatal,const char* format,...);
		uint32_t readSW();
		uint32_t readDW();
		uint32_t readVL();
		int eventReader();
		void trackChunkReader();
		void headerChunkReader();
		void chunkReader(int hdrXp);
	public:
		CMidiFile(const char* fn);
		~CMidiFile();
		const SEvent* getEvent(uint32_t id);
		uint32_t getEventCount();
		uint32_t getDivision();
		const char* getTitle();
		const char* getCopyright();
};
class CMidiPlayer
{
	private:
		CMidiFile *midiFile;
		uint32_t stamps[101];
		uint32_t ccstamps[101][16][132],ccc[16][132];//0..127:cc 128:pc 129:cp 130:pb 131:tempo
		uint16_t mute,solo;
		double ftime;
		fluid_settings_t* settings;
		fluid_synth_t* synth;
		fluid_audio_driver_t* adriver;
		uint32_t ctempo,ctsn,ctsd,dpt,divs,cks;
		//raw tempo, timesig num., timesig den., delay per tick, division, keysig
		//thread control
		uint32_t tceptr,tcpaused,tcstop;
		uint32_t finished,resumed;

		void setBit(uint16_t &n,uint16_t bn,uint16_t b);
		void fluidInitialize(const char* sf);
		void fluidDeinitialize();
		void processEvent(const SEvent *e);
		void processEventStub(const SEvent *e);
		void playEvents();
		void fileTimer1Pass();
		void fileTimer2Pass();
	public:
		CMidiPlayer();
		void playerLoadFile(const char* fn);
		void playerInit();
		void playerDeinit();
		void playerThread();
		void playerPanic();

		//playing control methods
		uint32_t getStamp(int id);
		uint32_t getTCeptr();
		void setTCeptr(uint32_t ep,uint32_t st);
		uint32_t getTCpaused();
		void setTCpaused(uint32_t ps);
		uint32_t isFinished();
		void setResumed();

		double getFtime();
		void getCurrentTimeSignature(int *n,int *d);
		void getCurrentKeySignature(int *ks);
		double getTempo();
		const char* getTitle();
		const char* getCopyright();

		void setGain(double gain);
		int getPolyphone();
		int getMaxPolyphone();
		void setMaxPolyphone(int p);

		void setChannelPreset(int ch,int b,int p);
		void getChannelPreset(int ch,int *b,int *p,char *name);
		void setMute(int ch,bool m);
		void setSolo(int ch,bool s);
		int getCC(int ch,int id);
		void setCC(int ch,int id,int val);
		void getReverbPara(double *r,double *d,double *w,double *l);
		void setReverbPara(int e,double r,double d,double w,double l);
		void getChorusPara(int *fb,double *l,double *r,double *d,int *type);
		void setChorusPara(int e,int fb,double l,double r,double d,int type);

		//void pushSoundFont(const char* url);
		int getSFCount();
		fluid_sfont_t* getSFPtr(int sfid);
};
#endif