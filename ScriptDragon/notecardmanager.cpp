#include "iostream"
#include "notecardmanager.h"
#include <QQmlEngine>
#include <QtQml>

NotecardManager::NotecardManager(QObject *parent) : QObject(parent) {
	//engine = new QQmlEngine( parent );
}

NotecardManager::~NotecardManager() {
	//delete engine;
}

Q_INVOKABLE void NotecardManager::addNotecard( QString newCardText, QString newCardTitle ) {
	std::cout << "addNotecard() called with newCardText=\"" << newCardText.toStdString().c_str() << "\" newCardTitle=\"" << newCardTitle.toStdString().c_str() << "\"" << std::endl;
	
	QQmlComponent component( engine, QUrl( QStringLiteral( "qrc:///TextNotecard.qml" ) ) );
	QObject* object = component.create();
	engine->setObjectOwnership( object, QQmlEngine::CppOwnership );
	notecards.append( object );
	std::cout << "notecards.size(): " << notecards.size() << std::endl;
	std::cout << "notecards: " << &notecards << std::endl;
	std::cout << "this: " << this << std::endl;
}

Q_INVOKABLE QList< QObject* > NotecardManager::getAllNotecards() {
	return notecards;
}

/*Q_INVOKABLE QQmlListProperty<QObject> NotecardManager::getAllNotecards() {
	allNotecards.clear();
	
	for( int i = 0; i < notecards.size(); ++i ) {
		allNotecards.append( notecards[ i ] );
	}
	
	return QQmlListProperty<QObject>( this, allNotecards );
}*/

Q_INVOKABLE void NotecardManager::removeNotecard( QObject* toRemove ) {
	std::cout << "to remove: " << toRemove << std::endl;
	std::cout << "notecards.size(): " << notecards.size() << std::endl;
	std::cout << "notecards: " << &notecards << std::endl;
	std::cout << "this: " << this << std::endl;
	
	for( unsigned int i = 0; i < notecards.size(); ++i ) {
		std::cout << notecards[ i ] << std::endl;
	}
	
	notecards.removeOne( toRemove );
}

void NotecardManager::setEngine( QQmlEngine* newEngine ) {
	engine = newEngine;
}
