#ifndef FILESAVERANDLOADER_H
#define FILESAVERANDLOADER_H

#include <QObject>
#include "notecardmanager.h"

class FileSaverAndLoader : public QObject
{
		Q_OBJECT
	public:
		explicit FileSaverAndLoader(QObject *parent = 0);
		
		Q_INVOKABLE void save( QUrl fileURL );
		
		Q_INVOKABLE void setNotecardManager( NotecardManager* newNM );
		
		Q_INVOKABLE void setScriptPage( QObject* newScriptPage );
		
	signals:
		
	public slots:
		
	private:
		NotecardManager* notecardManager;
		QObject* scriptPage;
};

//Define the singleton type provider function
static QObject* fileSaverAndLoader_provider( QQmlEngine* newEngine, QJSEngine* scriptEngine ) {
	Q_UNUSED( scriptEngine )
	
	FileSaverAndLoader* sf = new FileSaverAndLoader( newEngine );
	return sf;
}

#endif // FILESAVERANDLOADER_H
