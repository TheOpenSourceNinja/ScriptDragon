#ifndef NOTECARDMANAGER_H
#define NOTECARDMANAGER_H

#include <memory>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <QString>

class NotecardManager : public QObject
{
		Q_OBJECT
		//Q_PROPERTY( QQmlListProperty<QObject> allNotecards READ getAllNotecards NOTIFY notecardsChanged );
	public:
		explicit NotecardManager( QQmlEngine* newEngine = NULL, QObject *parent = NULL);
		~NotecardManager();
		
		Q_INVOKABLE void addNotecard( QString newCardText = "", QString newCardTitle = "");
		
		Q_INVOKABLE QList< QObject* > getAllNotecards();
		Q_INVOKABLE QObject* getCharactersPage();
		Q_INVOKABLE QList< QObject* > getNotecardsForCharacter( int characterID = 0 );
		
		Q_INVOKABLE void removeNotecard( QObject* toRemove );
		
		Q_INVOKABLE void setCharactersPage( QObject* newCharactersPage );
		
	signals:
		void notecardsChanged();
		
	public slots:
		
	private:
		QList< QObject* > notecards;
		//QList< QObject* > allNotecards;
		QQmlEngine* engine;
		QObject* charactersPage; //TODO: Try making this a property
};

//Define the singleton type provider function
static QObject* notecardManager_provider( QQmlEngine* newEngine, QJSEngine* scriptEngine ) {
	Q_UNUSED( scriptEngine )
	
	NotecardManager* nm = new NotecardManager( newEngine );
	return nm;
}

#endif // NOTECARDMANAGER_H
