#ifndef NOTECARDMANAGER_H
#define NOTECARDMANAGER_H

#include <climits>
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
		
		enum class associationType: uint_fast8_t {
			NONE = 0,
			CHARACTER,
			LOCATION,
			EVENT,
			SCRIPT_LINE,
			MISC
		};
		Q_ENUMS( associationType );
		
		Q_INVOKABLE void addNotecard( QString newCardTitle = "", QString newCardText = "", associationType assocType = associationType::NONE, int associatedID = INT_MAX );
		Q_INVOKABLE void associateNotecardWith( QObject* notecard, associationType assocType, int associatedID );
		
		void copyNotecardData( QObject* origin, QObject* destination );
		
		Q_INVOKABLE QList< QObject* > getAllNotecards();
		Q_INVOKABLE QObject* getCharactersPage();
		Q_INVOKABLE QObject* getNotecardsPage();
		Q_INVOKABLE QList< QObject* > getNotecardsForCharacter( int characterID = 0 );
		
		Q_INVOKABLE void removeAllNotecards();
		Q_INVOKABLE void removeNotecard( QObject* toRemove );
		
		Q_INVOKABLE void setCharactersPage( QObject* newCharactersPage );
		Q_INVOKABLE void setNotecardsPage( QObject* newNotecardsPage );
		
		Q_INVOKABLE void updateNotecard( QObject* origin );
		
	signals:
		void notecardsChanged();
		
	public slots:
		
	private:
		QList< QObject* > notecards;
		std::vector< std::vector< QList< QObject* > > > notecardsWithAssociations;
		std::vector< QObject* > notecardsWithDuplicates;
		QQmlEngine* engine;
		QObject* charactersPage; //TODO: Try making this a property
		QObject* notecardsPage;
		std::vector< uint_fast8_t > maxIDWithinAssociatedThing;
};

//Define the singleton type provider function
static QObject* notecardManager_provider( QQmlEngine* newEngine, QJSEngine* scriptEngine ) {
	Q_UNUSED( scriptEngine )
	
	NotecardManager* nm = new NotecardManager( newEngine );
	return nm;
}

#endif // NOTECARDMANAGER_H
