#ifndef FILESAVERANDLOADER_H
#define FILESAVERANDLOADER_H

#include <QObject>
#include <QUrl>
#include "notecardmanager.h"

class FileSaverAndLoader : public QObject
{
		Q_OBJECT
	public:
		explicit FileSaverAndLoader(QObject *parent = 0);
		
		Q_INVOKABLE void load( QUrl fileURL );
		
		Q_INVOKABLE void save( QUrl fileURL );
		Q_INVOKABLE void setCharactersPage( QObject* newCharactersPage );
		Q_INVOKABLE void setNotecardManager( NotecardManager* newNM );
		Q_INVOKABLE void setScriptPage( QObject* newScriptPage );
		
	signals:
		void fileLoaded( QUrl fileURL );
		
		void fileSaved( QUrl fileURL );
	public slots:
		
	private:
		NotecardManager* notecardManager;
		QObject* scriptPage;
		QObject* charactersPage;
		
		//extern Q_CORE_EXPORT int qt_ntfs_permission_lookup; //Increment to turn on NTFS permission checking, decrement to turn it off
};

//Define the singleton type provider function
static QObject* fileSaverAndLoader_provider( QQmlEngine* newEngine, QJSEngine* scriptEngine ) {
	Q_UNUSED( scriptEngine )
	
	FileSaverAndLoader* sf = new FileSaverAndLoader( newEngine );
	return sf;
}

#endif // FILESAVERANDLOADER_H
