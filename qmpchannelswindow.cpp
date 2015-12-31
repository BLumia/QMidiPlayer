#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include "qmpchannelswindow.hpp"
#include "ui_qmpchannelswindow.h"
#include "qmpmainwindow.hpp"

qmpChannelsWindow::qmpChannelsWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::qmpChannelsWindow)
{
	ui->setupUi(this);
	pselectw=new qmpPresetSelector(this);
	ceditw=new qmpChannelEditor(this);
	connect(this,SIGNAL(dialogClosing()),parent,SLOT(dialogClosed()));
	for(int i=0;i<16;++i)
	{
		ui->twChannels->setCellWidget(i,0,new QCheckBox(""));
		connect(ui->twChannels->cellWidget(i,0),SIGNAL(stateChanged(int)),this,SLOT(channelMSChanged()));
		ui->twChannels->setCellWidget(i,1,new QCheckBox(""));
		connect(ui->twChannels->cellWidget(i,1),SIGNAL(stateChanged(int)),this,SLOT(channelMSChanged()));
		ui->twChannels->setCellWidget(i,2,new QComboBox());
		QComboBox *cb=(QComboBox*)ui->twChannels->cellWidget(i,2);
		//stub
		cb->addItem("Internal fluidsynth");
		ui->twChannels->setCellWidget(i,3,new QDCLabel(""));
		((QDCLabel*)ui->twChannels->cellWidget(i,3))->setID(i);
		connect(ui->twChannels->cellWidget(i,3),SIGNAL(onDoubleClick(int)),this,SLOT(showPresetWindow(int)));
		ui->twChannels->setCellWidget(i,4,new QDCPushButton("..."));
		((QDCLabel*)ui->twChannels->cellWidget(i,4))->setID(i);
		connect(ui->twChannels->cellWidget(i,4),SIGNAL(onClick(int)),this,SLOT(showChannelEditorWindow(int)));
	}
	ui->twChannels->setColumnWidth(0,32);
	ui->twChannels->setColumnWidth(1,32);
	ui->twChannels->setColumnWidth(2,192);
	ui->twChannels->setColumnWidth(3,192);
	ui->twChannels->setColumnWidth(4,32);
}

void qmpChannelsWindow::closeEvent(QCloseEvent *event)
{
	setVisible(false);
	emit dialogClosing();
	event->accept();
}

void qmpChannelsWindow::channelWindowsUpdate()
{
	for(int i=0;i<16;++i)
	{
		char data[128],nm[24];
		int b,p;
		qmpMainWindow::getInstance()->getPlayer()->getChannelPreset(i,&b,&p,nm);
		sprintf(data,"%d:%d %s",b,p,nm);
		((QLabel*)ui->twChannels->cellWidget(i,3))->setText(data);
	}
}

void qmpChannelsWindow::channelMSChanged()
{
	for(int i=0;i<16;++i)
	{
		QCheckBox *m,*s;
		m=(QCheckBox*)ui->twChannels->cellWidget(i,0);
		s=(QCheckBox*)ui->twChannels->cellWidget(i,1);
		if(m->isChecked()&&s->isChecked())s->setChecked(false);
		qmpMainWindow::getInstance()->getPlayer()->setMute(i,m->isChecked());
		qmpMainWindow::getInstance()->getPlayer()->setSolo(i,s->isChecked());
	}
}

qmpChannelsWindow::~qmpChannelsWindow()
{
	delete ui;
}

void qmpChannelsWindow::on_pbUnmute_clicked()
{
	for(int i=0;i<16;++i)
	{
		((QCheckBox*)ui->twChannels->cellWidget(i,0))->setChecked(false);
		qmpMainWindow::getInstance()->getPlayer()->setMute(i,false);
	}
}

void qmpChannelsWindow::on_pbUnsolo_clicked()
{
	for(int i=0;i<16;++i)
	{
		((QCheckBox*)ui->twChannels->cellWidget(i,1))->setChecked(false);
		qmpMainWindow::getInstance()->getPlayer()->setSolo(i,false);
	}
}

void qmpChannelsWindow::showPresetWindow(int chid)
{
	pselectw->show();
	pselectw->setupWindow(chid);
}

void qmpChannelsWindow::showChannelEditorWindow(int chid)
{
	ceditw->show();
	ceditw->setupWindow(chid);
}