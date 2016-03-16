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
		explicit NotecardManager(QObject *parent = NULL);
		~NotecardManager();
		
		Q_INVOKABLE void addNotecard( QString newCardText = "", QString newCardTitle = "" );
		
		Q_INVOKABLE QList< QObject* > getAllNotecards();
		//Q_INVOKABLE QQmlListProperty<QObject> getAllNotecards();
		
		Q_INVOKABLE void removeNotecard( QObject* toRemove );
		
		void setEngine( QQmlEngine* newEngine );
		
	signals:
		void notecardsChanged();
		
	public slots:
		
	private:
		QList< QObject* > notecards;
		QList< QObject* > allNotecards;
		QQmlEngine* engine;
};

//Define the singleton type provider function
static QObject* notecardManager_provider( QQmlEngine* newEngine, QJSEngine* scriptEngine ) {
	Q_UNUSED( scriptEngine )
	
	NotecardManager* nm = new NotecardManager();
	nm->setEngine( newEngine );
	return nm;
}

#endif // NOTECARDMANAGER_H
