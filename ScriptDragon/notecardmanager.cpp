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
	std::cout << "addNotecard() called with newCardText=\"" << newCardText.toStdString().c_str() << "\" newCardTitle=\"" << newCardTitle.toStdString().c_str() << "\"" << std::endl;
	
	QQmlComponent component( engine, QUrl( QStringLiteral( "qrc:///TextNotecard.qml" ) ) );
	
	while( component.status() == QQmlComponent::Loading ) {};
	
	if( component.status() == QQmlComponent::Error ) {
		std::cerr << "Error creating component" << std::endl;
	} else {
		QObject* object = component.create();
		engine->setObjectOwnership( object, QQmlEngine::CppOwnership );
		//object->setProperty( "charactersTab", charactersTab );
		object->dumpObjectInfo();
		notecards.append( object );
		std::cerr.flush();
		std::cout << "notecards.size(): " << notecards.size() << std::endl;
		std::cout << "notecards: " << &notecards << std::endl;
		std::cout << "this: " << this << std::endl;
	}
}

Q_INVOKABLE QList< QObject* > NotecardManager::getAllNotecards() {
	for( decltype( notecards )::size_type i = 0; i < notecards.size(); ++i ) {
		std::cout << notecards[ i ]->property("text").toString().toStdString().c_str() << std::endl;
	}
	
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
	std::cout << "to remove: " << toRemove << std::endl;
	std::cout << "notecards.size(): " << notecards.size() << std::endl;
	std::cout << "notecards: " << &notecards << std::endl;
	std::cout << "this: " << this << std::endl;
	
	for( decltype( notecards )::size_type i = 0; i < notecards.size(); ++i ) {
		std::cout << notecards[ i ] << std::endl;
	}
	
	notecards.removeOne( toRemove );
}

Q_INVOKABLE void NotecardManager::setCharactersPage( QObject* newCharactersPage ) {
	std::cout << "characters page: " << newCharactersPage << std::endl;
	if( charactersPage != NULL ) {
		engine->setObjectOwnership( charactersPage, QQmlEngine::JavaScriptOwnership );
	}
	
	charactersPage = newCharactersPage;
	
	if( charactersPage != NULL ) {
		engine->setObjectOwnership( charactersPage, QQmlEngine::CppOwnership );
	}
}
