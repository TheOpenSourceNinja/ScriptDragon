#include "iostream"
#include "notecardmanager.h"
#include <QQmlEngine>
#include <QtQml>

NotecardManager::NotecardManager( QQmlEngine* newEngine, QObject *parent ) : QObject(parent) {
	engine = newEngine;
}

NotecardManager::~NotecardManager() {
	//delete engine;
}

Q_INVOKABLE void NotecardManager::addNotecard( QString newCardText, QString newCardTitle ) {
	//std::cout << "addNotecard() called with newCardText=\"" << newCardText.toStdString().c_str() << "\" newCardTitle=\"" << newCardTitle.toStdString().c_str() << "\"" << std::endl;
	
	QQmlComponent component( engine, QUrl( QStringLiteral( "qrc:///TextNotecard.qml" ) ) );
	
	while( component.status() == QQmlComponent::Loading ) {};
	
	if( component.status() == QQmlComponent::Error ) {
		std::cerr << "Error creating component" << std::endl;
	} else {
		QObject* object = component.create();
		engine->setObjectOwnership( object, QQmlEngine::CppOwnership );
		//object->setProperty( "charactersTab", charactersTab );
		notecards.append( object );
	}
}

Q_INVOKABLE QList< QObject* > NotecardManager::getAllNotecards() {
	return notecards;
}

Q_INVOKABLE QObject* NotecardManager::getCharactersPage() {
	return charactersPage;
}

Q_INVOKABLE QList< QObject* > NotecardManager::getNotecardsForCharacter( int characterID ) {
	QList< QObject* > results;
	
	for( decltype( notecards )::size_type i = 0; i < notecards.size(); ++i ) {
		std::cout << notecards[ i ]->property( "associatedCharacterId" ).toInt() << std::endl;
		std::cout << notecards[ i ]->property( "associatedCharacterName" ).toString().toStdString().c_str() << std::endl;
		
		if( characterID == notecards[ i ]->property( "associatedCharacterId" ).toInt() ) {
			results.append( notecards[ i ] );
		}
	}
	
	return results;
}

/*Q_INVOKABLE QQmlListProperty<QObject> NotecardManager::getAllNotecards() {
	allNotecards.clear();
	
	for( int i = 0; i < notecards.size(); ++i ) {
		allNotecards.append( notecards[ i ] );
	}
	
	return QQmlListProperty<QObject>( this, allNotecards );
}*/

Q_INVOKABLE void NotecardManager::removeNotecard( QObject* toRemove ) {
	notecards.removeOne( toRemove );
}

Q_INVOKABLE void NotecardManager::setCharactersPage( QObject* newCharactersPage ) {
	if( charactersPage != NULL ) {
		engine->setObjectOwnership( charactersPage, QQmlEngine::JavaScriptOwnership );
	}
	
	charactersPage = newCharactersPage;
	
	if( charactersPage != NULL ) {
		engine->setObjectOwnership( charactersPage, QQmlEngine::CppOwnership );
	}
}
