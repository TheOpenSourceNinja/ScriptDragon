#include "iostream"
#include "notecardmanager.h"
#include <QQmlEngine>
#include <QtQml>

NotecardManager::NotecardManager( QQmlEngine* newEngine, QObject *parent ) : QObject(parent) {
	engine = newEngine;
	charactersPage = NULL;
	notecardsPage = NULL;
}

NotecardManager::~NotecardManager() {
	//delete engine;
}

Q_INVOKABLE void NotecardManager::addNotecard( QString newCardTitle, QString newCardText, associationType assocType, int associatedID ) {
	std::cout << "addNotecard() called with newCardTitle=\"" << newCardTitle.toStdString().c_str() << "\" newCardText=\"" << newCardText.toStdString().c_str() << "\" assocType=" << assocType << " assocatedID=" << associatedID << std::endl;
	
	QQmlComponent component( engine, QUrl( QStringLiteral( "qrc:///TextNotecard.qml" ) ) );
	
	while( component.status() == QQmlComponent::Loading ) {};
	
	if( component.status() == QQmlComponent::Error ) {
		std::cerr << "Error creating component" << std::endl;
	} else {
		QObject* object = component.create();
		engine->setObjectOwnership( object, QQmlEngine::CppOwnership );
		//object->setProperty( "charactersTab", charactersTab );
		notecards.append( object );
		object->setProperty( "text", newCardText );
		object->setProperty( "title", newCardTitle );
		object->setProperty( "associationType", assocType);
		object->setProperty( "associatedID", associatedID );
		
		if( associatedID < INT_MAX ) {
			associateNotecardWith( object, assocType, associatedID );
		}
		
		emit notecardsChanged();
	}
}

Q_INVOKABLE void NotecardManager::associateNotecardWith( QObject* notecard, associationType assocType, int associatedID ) {
	QQmlComponent component( engine, QUrl( QStringLiteral( "qrc:///TextNotecard.qml" ) ) );
	
	while( component.status() == QQmlComponent::Loading ) {};
	
	if( component.status() == QQmlComponent::Error ) {
		std::cerr << "Error creating component" << std::endl;
	} else {
		QObject* object2 = component.create();
		engine->setObjectOwnership( object2, QQmlEngine::CppOwnership );
		//object->setProperty( "charactersTab", charactersTab );
		//notecards.append( object2 );
		
		if( assocType >= notecardsWithAssociations.size() ) {
			notecardsWithAssociations.resize( assocType + 1 );
		}
		
		if( associatedID >= notecardsWithAssociations[ assocType ].size() ) {
			notecardsWithAssociations[ assocType ].resize( associatedID + 1 );
		}
		
		notecardsWithAssociations[ assocType ][ associatedID ].append( object2 );
		object2->setProperty( "text", notecard->property( "text" ) );
		//object2->setProperty( "title", newCardTitle );
		object2->setProperty( "associationType", assocType);
		object2->setProperty( "associatedID", associatedID );
		
		emit notecardsChanged();
	}
}

Q_INVOKABLE QList< QObject* > NotecardManager::getAllNotecards() {
	QList< QObject* > results;
	
	for( decltype( notecards )::size_type i = 0; i < notecards.size(); ++i ) {
		std::cout << notecards[ i ]->property( "associatedID" ).toInt() << std::endl;
		std::cout << notecards[ i ]->property( "associatedText" ).toString().toStdString().c_str() << std::endl;
		
		//if( notecards[ i ]->property( "associationType" ) == CHARACTER && characterID == notecards[ i ]->property( "associatedID" ).toInt() ) {
			results.append( notecards[ i ] );
			std::cout << "append notecard" << std::endl;
		//} else {
		//	std::cout << "not appending notecard" << std::endl;
		//}
	}
	
	return results;
}

Q_INVOKABLE QObject* NotecardManager::getCharactersPage() {
	return charactersPage;
}

Q_INVOKABLE QObject* NotecardManager::getNotecardsPage() {
	return notecardsPage;
}

Q_INVOKABLE QList< QObject* > NotecardManager::getNotecardsForCharacter( int characterID ) {
	QList< QObject* > results;
	
	/*for( decltype( notecards )::size_type i = 0; i < notecards.size(); ++i ) {
		std::cout << notecards[ i ]->property( "associatedID" ).toInt() << std::endl;
		std::cout << notecards[ i ]->property( "associatedText" ).toString().toStdString().c_str() << std::endl;
		
		if( notecards[ i ]->property( "associationType" ) == CHARACTER && characterID == notecards[ i ]->property( "associatedID" ).toInt() ) {
			results.append( notecards[ i ] );
			std::cout << "append notecard" << std::endl;
		} else {
			std::cout << "not appending notecard" << std::endl;
		}
	}*/
	
	if( notecardsWithAssociations.size() > CHARACTER ) {
		if( notecardsWithAssociations[ CHARACTER ].size() > characterID ) {
			results = notecardsWithAssociations[ CHARACTER ][ characterID ];
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
	
	for( unsigned int i = 0; i < notecardsWithAssociations.size(); ++i ) {
		for( unsigned int j = 0; j < notecardsWithAssociations[ i ].size(); ++j ) {
			if( notecardsWithAssociations[ i ][ j ].removeOne( toRemove ) ) {
				break;
			}
		}
	}
	
	emit notecardsChanged();
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

Q_INVOKABLE void NotecardManager::setNotecardsPage( QObject* newNotecardsPage ) {
	if( notecardsPage != NULL ) {
		engine->setObjectOwnership( notecardsPage, QQmlEngine::JavaScriptOwnership );
	}
	
	notecardsPage = newNotecardsPage;
	
	if( notecardsPage != NULL ) {
		engine->setObjectOwnership( notecardsPage, QQmlEngine::CppOwnership );
	}
}