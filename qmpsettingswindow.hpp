#ifndef QMPSETTINGSWINDOW_H
#define QMPSETTINGSWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QSettings>
#include <QListWidget>

namespace Ui {
	class qmpSettingsWindow;
}

class qmpSettingsWindow:public QDialog
{
	Q_OBJECT

	public:
		explicit qmpSettingsWindow(QWidget *parent=0);
		~qmpSettingsWindow();
		void closeEvent(QCloseEvent *event);
		void settingsInit();
		QListWidget* getSFWidget();
	signals:
		void dialogClosing();

	private slots:
		void on_buttonBox_accepted();
		void on_buttonBox_rejected();

		void on_cbBufSize_currentTextChanged(const QString &s);
		void on_cbBufCnt_currentTextChanged(const QString &s);

		void on_pbAdd_clicked();
		void on_pbRemove_clicked();
		void on_pbUp_clicked();
		void on_pbDown_clicked();

		void on_cbAutoBS_stateChanged();

		private:
		Ui::qmpSettingsWindow *ui;
		void settingsUpdate();
		static QSettings *settings;
	public: static QSettings* getSettingsIntf(){return settings;}
};

#endif // QMPSETTINGSWINDOW_H
