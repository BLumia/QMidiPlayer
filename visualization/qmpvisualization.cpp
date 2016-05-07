#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "qmpvisualization.hpp"

int viewdist=100;
int notestretch=100;//length of quarter note
int minnotelength=100;
int noteappearance=1,showpiano=1,stairpiano=1,savevp=1;
int wwidth=800,wheight=600,wsupersample=1,wmultisample=1;
int fov=60,vsync=1,tfps=60;
DWORD iccolors[]={0XFFFF0000,0XFFFF8000,0XFFFFBF00,0XFFFFFF00,
				  0XFFBFFF00,0XFF80FF00,0XFF00FF00,0XFF00FFBF,
				  0XFF00FFFF,0XFF333333,0XFF00BFFF,0XFF007FFF,
				  0XFF0000FF,0XFF7F00FF,0XFFBF00FF,0XFFFF00BF};
DWORD accolors[]={0XFFFF9999,0XFFFFCC99,0XFFFFEE99,0XFFFFFF99,
				  0XFFEEFF99,0XFFCCFF99,0XFF99FF99,0XFF99FFCC,
				  0XFF99FFFF,0XFF999999,0XFF99EEFF,0XFF99CCFF,
				  0XFF9999FF,0XFFCC99FF,0XFFEE99FF,0XFFFF99EE};

bool cmp(MidiVisualEvent* a,MidiVisualEvent* b)
{
	if(a->tcs<b->tcs)return true;if(a->tcs>b->tcs)return false;
	if(a->tce<b->tce)return true;return false;
}
void CReaderCallBack::callBack(void *callerdata,void *)
{
	SEventCallBackData* cbd=(SEventCallBackData*)callerdata;
	switch(cbd->type&0xF0)
	{
		case 0x80:
			par->pushNoteOff(cbd->time,cbd->type&0x0F,cbd->p1);
		break;
		case 0x90:
			par->pushNoteOn(cbd->time,cbd->type&0x0F,cbd->p1,cbd->p2);
		break;
	}
}
void CHandlerCallBack::callBack(void*,void*)
{
	if(par->ctk>par->api->getCurrentTimeStamp()+par->api->getDivision()/3)
		par->elb=0;
	par->ctk=par->api->getCurrentTimeStamp();
}
void qmpVisualization::showThread()
{
	wwidth=api->getOptionInt("Visualization/wwidth");
	wheight=api->getOptionInt("Visualization/wheight");
	wsupersample=api->getOptionInt("Visualization/supersampling");
	wmultisample=api->getOptionInt("Visualization/multisampling");
	fov=api->getOptionInt("Visualization/fov");
	noteappearance=api->getOptionBool("Visualization/3dnotes");
	showpiano=api->getOptionBool("Visualization/showpiano");
	stairpiano=api->getOptionBool("Visualization/stairpiano");
	savevp=api->getOptionBool("Visualization/savevp");
	vsync=api->getOptionBool("Visualization/vsync");
	tfps=api->getOptionInt("Visualization/tfps");
	viewdist=api->getOptionInt("Visualization/viewdist");
	notestretch=api->getOptionInt("Visualization/notestretch");
	minnotelength=api->getOptionInt("Visualization/minnotelen");
	sm=smGetInterface(SMELT_APILEVEL);
	sm->smVidMode(wwidth,wheight,true);
	sm->smUpdateFunc(h);sm->smQuitFunc(closeh);
	sm->smWinTitle("QMidiPlayer Visualization");
	sm->smSetFPS(vsync?FPS_VSYNC:tfps);
	sm->smNoSuspend(true);
	sm->smInit();shouldclose=false;
	sm->smTextureOpt(TPOT_POT,TFLT_LINEAR);
	chequer=sm->smTextureLoad("chequerboard.png");if(!chequer)
	chequer=sm->smTextureLoad("/usr/share/qmidiplayer/img/chequerboard.png");
	bgtex=sm->smTextureLoad(api->getOptionString("Visualization/background").c_str());
	if(showpiano)for(int i=0;i<16;++i)p3d[i]=new qmpVirtualPiano3D();
	memset(traveld,0,sizeof(traveld));
	if(noteappearance==1)nebuf=new smEntity3DBuffer();else nebuf=NULL;
	tdscn=sm->smTargetCreate(wwidth*wsupersample,wheight*wsupersample,wmultisample);
	if(!font.loadTTF("/usr/share/fonts/truetype/freefont/FreeMono.ttf",16))
	printf("W: Font load failed.\n");
	if(!font2.loadTTF("/usr/share/fonts/truetype/wqy/wqy-microhei.ttc",16))
	printf("W: Font load failed.\n");
	pos[0]=0;pos[1]=120;pos[2]=70;
	rot[0]=0;rot[1]=75;rot[2]=90;ctk=0;
	if(savevp)
	{
		pos[0]=api->getOptionDouble("Visualization/px");
		pos[1]=api->getOptionDouble("Visualization/py");
		pos[2]=api->getOptionDouble("Visualization/pz");
		rot[0]=api->getOptionDouble("Visualization/rx");
		rot[1]=api->getOptionDouble("Visualization/ry");
		rot[2]=api->getOptionDouble("Visualization/rz");
	}
	sm->smMainLoop();
}
void qmpVisualization::show()
{
	rendererTh=new std::thread(&qmpVisualization::showThread,this);
}
void qmpVisualization::close()
{
	shouldclose=true;
	rendererTh->join();delete rendererTh;

	if(showpiano)for(int i=0;i<16;++i)delete p3d[i];
	if(noteappearance==1)delete nebuf;
	sm->smFinale();
	if(savevp)
	{
		api->setOptionDouble("Visualization/px",pos[0]);
		api->setOptionDouble("Visualization/py",pos[1]);
		api->setOptionDouble("Visualization/pz",pos[2]);
		api->setOptionDouble("Visualization/rx",rot[0]);
		api->setOptionDouble("Visualization/ry",rot[1]);
		api->setOptionDouble("Visualization/rz",rot[2]);
	}
	font.releaseTTF();
	font2.releaseTTF();
	sm->smTextureFree(chequer);
	if(bgtex)sm->smTextureFree(bgtex);
	sm->smTargetFree(tdscn);
	sm->smRelease();
}
void qmpVisualization::reset()
{
	for(unsigned i=0;i<pool.size();++i)delete pool[i];
	pool.clear();elb=ctk=0;
	for(int i=0;i<16;++i)for(int j=0;j<128;++j)
	{
		while(!pendingt[i][j].empty())pendingt[i][j].pop();
		while(!pendingv[i][j].empty())pendingv[i][j].pop();
	}
}
void qmpVisualization::start(){playing=true;std::sort(pool.begin(),pool.end(),cmp);}
void qmpVisualization::stop(){playing=false;}
void qmpVisualization::pause(){playing=!playing;}
bool qmpVisualization::update()
{
	smQuad q;
	for(int i=0;i<4;++i)
	{q.v[i].col=0xFF999999;q.v[i].z=showpiano?-5:0;}
	q.tex=chequer;q.blend=BLEND_ALPHABLEND;
	q.v[0].x=q.v[3].x=-120;q.v[1].x=q.v[2].x=120;
	q.v[0].y=q.v[1].y=-120;q.v[2].y=q.v[3].y=120;
	q.v[0].tx=q.v[3].tx=0;q.v[1].tx=q.v[2].tx=30;
	q.v[0].ty=q.v[1].ty=0;q.v[2].ty=q.v[3].ty=30;
	sm->smRenderBegin3D(fov,tdscn);
	sm->sm3DCamera6f2v(pos,rot);
	sm->smClrscr(0,1,1);
	sm->smRenderQuad(&q);
	if(sm->smGetKeyState(SMK_D))pos[0]+=cos(smMath::deg2rad(rot[2]-90)),pos[1]+=sin(smMath::deg2rad(rot[2]-90));
	if(sm->smGetKeyState(SMK_A))pos[0]-=cos(smMath::deg2rad(rot[2]-90)),pos[1]-=sin(smMath::deg2rad(rot[2]-90));
	if(sm->smGetKeyState(SMK_S))pos[0]+=cos(smMath::deg2rad(rot[2])),pos[1]+=sin(smMath::deg2rad(rot[2]));
	if(sm->smGetKeyState(SMK_W))pos[0]-=cos(smMath::deg2rad(rot[2])),pos[1]-=sin(smMath::deg2rad(rot[2]));
	if(sm->smGetKeyState(SMK_Q))pos[2]+=1;
	if(sm->smGetKeyState(SMK_E))pos[2]-=1;
	if(sm->smGetKeyState(SMK_LBUTTON)==SMKST_HIT)
	sm->smSetMouseGrab(true),sm->smGetMouse2f(&lastx,&lasty);
	if(sm->smGetKeyState(SMK_LBUTTON)==SMKST_KEEP)
	{
		float x,y;
		sm->smGetMouse2f(&x,&y);
		rot[1]-=(y-lasty)*0.01;
		rot[2]+=(x-lastx)*0.01;
		while(rot[1]>360)rot[1]-=360;
		while(rot[1]<0)rot[1]+=360;
		while(rot[2]>360)rot[2]-=360;
		while(rot[2]<0)rot[2]+=360;
	}
	if(sm->smGetKeyState(SMK_LBUTTON)==SMKST_RELEASE)
	sm->smSetMouseGrab(false);
	if(sm->smGetKeyState(SMK_I))rot[1]+=1;
	if(sm->smGetKeyState(SMK_K))rot[1]-=1;
	if(sm->smGetKeyState(SMK_L))rot[0]+=1;
	if(sm->smGetKeyState(SMK_J))rot[0]-=1;
	if(sm->smGetKeyState(SMK_U))rot[2]+=1;
	if(sm->smGetKeyState(SMK_O))rot[2]-=1;
	//printf("pos: %f %f %f\n",pos[0],pos[1],pos[2]);
	//printf("rot: %f %f %f\n",rot[0],rot[1],rot[2]);
	double lpt=(double)notestretch/api->getDivision()/10.;
	memset(notestatus,0,sizeof(notestatus));
	for(uint32_t i=elb;i<pool.size();++i)
	{
		if(((double)pool[i]->tcs-ctk)*lpt>viewdist*2)break;
		if(fabs((double)pool[i]->tcs-ctk)*lpt<viewdist*2||fabs((double)pool[i]->tce-ctk)*lpt<viewdist*2)
		{
			if(api->getChannelMask(pool[i]->ch))continue;
			smvec3d a(0.63*((double)pool[i]->key-64)+.1,(stairpiano?(56-pool[i]->ch*7.):(64-pool[i]->ch*8.)),((double)pool[i]->tce-ctk)*lpt+stairpiano*pool[i]->ch*2.);
			smvec3d b(0.63*((double)pool[i]->key-64)+.7,(stairpiano?(56-pool[i]->ch*7.):(64-pool[i]->ch*8.))+.4,((double)pool[i]->tcs-ctk)*lpt+stairpiano*pool[i]->ch*2.);
			bool isnoteon=pool[i]->tcs<=ctk&&pool[i]->tce>=ctk;if(isnoteon)
			a.x=0.63*((double)pool[i]->key-64+api->getPitchBend(pool[i]->ch))+.1,
			b.x=0.63*((double)pool[i]->key-64+api->getPitchBend(pool[i]->ch))+.7;
			notestatus[pool[i]->ch][pool[i]->key]|=isnoteon;a.x*=1.2;b.x*=1.2;
			if(((double)pool[i]->tce-pool[i]->tcs)*lpt<minnotelength/100.)a.z=((double)pool[i]->tcs-ctk)*lpt-minnotelength/100.+stairpiano*pool[i]->ch*2;
			drawCube(a,b,SETA(isnoteon?accolors[pool[i]->ch]:iccolors[pool[i]->ch],int(pool[i]->vel*(isnoteon?2.0:1.6))),0);
		}
	}
	if(noteappearance)nebuf->drawBatch();
	if(showpiano)
	for(int i=0;i<16;++i)
	{
		for(int j=0;j<128;++j)
		{
			if(notestatus[i][j])
			if(traveld[i][j]<10)traveld[i][j]+=2;else traveld[i][j]=10;
			else
			if(traveld[i][j]>0)traveld[i][j]-=2;else traveld[i][j]=0;
			p3d[i]->setKeyTravelDist(j,traveld[i][j]/10.);
		}
		p3d[i]->render(smvec3d(0.756*api->getPitchBend(i),stairpiano?55-i*7:62-i*8,stairpiano*i*2));
	}
	if(playing)ctk+=(int)(1e6/(api->getRawTempo()/api->getDivision())*sm->smGetDelta());
	while(pool.size()&&elb<pool.size()&&((double)ctk-pool[elb]->tce)*lpt>viewdist*2)++elb;
	sm->smRenderEnd();
	sm->smRenderBegin2D();
	sm->smClrscr(0xFF666666);q.blend=BLEND_ALPHABLEND;
	for(int i=0;i<4;++i){q.v[i].col=0xFFFFFFFF;q.v[i].z=0;}
	if(bgtex)
	{
		q.tex=bgtex;
		q.v[0].x=q.v[3].x=0;q.v[1].x=q.v[2].x=wwidth;
		q.v[0].y=q.v[1].y=0;q.v[2].y=q.v[3].y=wheight;
		q.v[0].tx=q.v[3].tx=0;q.v[1].tx=q.v[2].tx=1;
		q.v[0].ty=q.v[1].ty=0;q.v[2].ty=q.v[3].ty=1;
		sm->smRenderQuad(&q);
	}
	q.tex=sm->smTargetTexture(tdscn);
	q.v[0].tx=q.v[3].tx=0;q.v[1].tx=q.v[2].tx=1;
	q.v[0].ty=q.v[1].ty=0;q.v[2].ty=q.v[3].ty=1;
	q.v[0].x=q.v[1].x=0;q.v[2].x=q.v[3].x=wwidth;
	q.v[0].y=q.v[3].y=0;q.v[1].y=q.v[2].y=wheight;
	sm->smRenderQuad(&q);
	wchar_t ws[1024];memset(ws,0,sizeof(ws));
	mbstowcs(ws,api->getTitle().c_str(),1024);
	font2.updateString(L"Title: %ls",ws);
	font2.render(1,wheight-64,0xFFFFFFFF,ALIGN_LEFT);
	font2.render(0,wheight-65,0xFF000000,ALIGN_LEFT);
	font.updateString(L"Tempo: %.2f",api->getRealTempo());
	font.render(1,wheight-44,0xFFFFFFFF,ALIGN_LEFT);
	font.render(0,wheight-45,0xFF000000,ALIGN_LEFT);
	font.updateString(L"Current tick: %d",ctk);
	font.render(1,wheight-24,0xFFFFFFFF,ALIGN_LEFT);
	font.render(0,wheight-25,0xFF000000,ALIGN_LEFT);
	font.updateString(L"FPS: %.2f",sm->smGetFPS());
	font.render(1,wheight-4,0xFFFFFFFF,ALIGN_LEFT);
	font.render(0,wheight-5,0xFF000000,ALIGN_LEFT);
	sm->smRenderEnd();
	return shouldclose;
}

void qmpVisualization::drawCube(smvec3d a,smvec3d b,DWORD col,SMTEX tex)
{
	smQuad q;q.blend=BLEND_ALPHABLEND;
	q.tex=tex;for(int i=0;i<4;++i)q.v[i].col=col;
	if(noteappearance==1)
	{
		smMatrix I;I.loadIdentity();smEntity3D c=smEntity3D::cube(a,b,col);
		nebuf->addTransformedEntity(&c,I,smvec3d(0,0,0));
	}
	else
	{
		q.v[0].x=a.x;q.v[0].y=b.y;q.v[0].z=a.z;
		q.v[1].x=b.x;q.v[1].y=b.y;q.v[1].z=a.z;
		q.v[2].x=b.x;q.v[2].y=b.y;q.v[2].z=b.z;
		q.v[3].x=a.x;q.v[3].y=b.y;q.v[3].z=b.z;
		sm->smRenderQuad(&q);
	}
}

qmpVisualization::qmpVisualization(qmpPluginAPI* _api){api=_api;}
qmpVisualization::~qmpVisualization(){api=NULL;}
void qmpVisualization::init()
{
	cb=new CReaderCallBack(this);
	hcb=new CHandlerCallBack(this);
	vi=new CDemoVisualization(this);
	h=new CMidiVisualHandler(this);
	closeh=new CloseHandler(this);
	hvif=api->registerVisualizationIntf(vi);
	herif=api->registerEventReaderIntf(cb,NULL);
	hehif=api->registerEventHandlerIntf(hcb,NULL);
	api->registerOptionBool("Visualization-Appearance","Show Piano","Visualization/showpiano",true);
	api->registerOptionBool("Visualization-Appearance","3D Notes","Visualization/3dnotes",true);
	api->registerOptionBool("Visualization-Appearance","Arrange channels on a stair","Visualization/stairpiano",true);
	api->registerOptionBool("Visualization-Video","Enable VSync","Visualization/vsync",true);
	api->registerOptionBool("Visualization-Video","Save Viewport","Visualization/savevp",true);
	api->registerOptionInt("Visualization-Video","Window Width","Visualization/wwidth",320,3200,800);
	api->registerOptionInt("Visualization-Video","Window Height","Visualization/wheight",320,3200,600);
	api->registerOptionInt("Visualization-Video","FPS","Visualization/tfps",5,1000,60);
	api->registerOptionInt("Visualization-Video","Supersampling","Visualization/supersampling",1,16,0);
	api->registerOptionInt("Visualization-Video","Multisampling","Visualization/multisampling",0,16,0);
	api->registerOptionInt("Visualization-Video","FOV","Visualization/fov",30,180,60);
	api->registerOptionInt("Visualization-Appearance","View distance","Visualization/viewdist",20,1000,100);
	api->registerOptionInt("Visualization-Appearance","Note stretch","Visualization/notestretch",20,500,100);
	api->registerOptionInt("Visualization-Appearance","Minimum note length","Visualization/minnotelen",20,500,100);
	api->registerOptionString("Visualization-Appearance","Background Image","Visualization/background","");
	api->registerOptionDouble("","","Visualization/px",-999999999,999999999,0);
	api->registerOptionDouble("","","Visualization/py",-999999999,999999999,120);
	api->registerOptionDouble("","","Visualization/pz",-999999999,999999999,70);
	api->registerOptionDouble("","","Visualization/rx",-999999999,999999999,0);
	api->registerOptionDouble("","","Visualization/ry",-999999999,999999999,75);
	api->registerOptionDouble("","","Visualization/rz",-999999999,999999999,90);
	wwidth=api->getOptionInt("Visualization/wwidth");
	wheight=api->getOptionInt("Visualization/wheight");
	wsupersample=api->getOptionInt("Visualization/supersampling");
	wmultisample=api->getOptionInt("Visualization/multisampling");
	fov=api->getOptionInt("Visualization/fov");
	noteappearance=api->getOptionBool("Visualization/3dnotes");
	showpiano=api->getOptionBool("Visualization/showpiano");
	stairpiano=api->getOptionBool("Visualization/stairpiano");
	savevp=api->getOptionBool("Visualization/savevp");
	vsync=api->getOptionBool("Visualization/vsync");
	tfps=api->getOptionInt("Visualization/tfps");
	viewdist=api->getOptionInt("Visualization/viewdist");
	notestretch=api->getOptionInt("Visualization/notestretch");
	minnotelength=api->getOptionInt("Visualization/minnotelen");
}
void qmpVisualization::deinit()
{
	if(!api)return;close();
	for(unsigned i=0;i<pool.size();++i)delete pool[i];pool.clear();
	api->unregisterVisualizationIntf(hvif);
	api->unregisterEventReaderIntf(herif);
	api->unregisterEventHandlerIntf(hehif);
	delete cb;delete hcb;delete vi;
	delete h;delete closeh;
}
const char* qmpVisualization::pluginGetName()
{return "QMidiPlayer Default Visualization Plugin";}
const char* qmpVisualization::pluginGetVersion()
{return "0.8.1";}

void qmpVisualization::pushNoteOn(uint32_t tc,uint32_t ch,uint32_t key,uint32_t vel)
{
	pendingt[ch][key].push(tc);
	pendingv[ch][key].push(vel);
}
void qmpVisualization::pushNoteOff(uint32_t tc,uint32_t ch,uint32_t key)
{
	if(pendingt[ch][key].size()<1)return;
	MidiVisualEvent *ne=new MidiVisualEvent();
	ne->tcs=pendingt[ch][key].top();pendingt[ch][key].pop();
	ne->tce=tc;ne->ch=ch;ne->key=key;
	ne->vel=pendingv[ch][key].top();pendingv[ch][key].pop();
	pool.push_back(ne);
	if(tc>fintk)fintk=tc;
}
