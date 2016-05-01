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
	std::cout << "addNotecard() called with newCardTitle=\"" << newCardTitle.toStdString() << "\" newCardText=\"" << newCardText.toStdString() << "\" assocType=" << ( uint_fast8_t ) assocType << " assocatedID=" << associatedID << std::endl;
	
	QQmlComponent component( engine, QUrl( QStringLiteral( "qrc:///TextNotecard.qml" ) ) );
	
	while( Q_UNLIKELY( component.status() == QQmlComponent::Loading ) ) {};
	
	if( Q_UNLIKELY( component.status() == QQmlComponent::Error ) ) {
		std::cerr << "Error creating component" << std::endl;
	} else {
		QObject* object = component.create();
		engine->setObjectOwnership( object, QQmlEngine::CppOwnership );
		//object->setProperty( "charactersTab", charactersTab );
		notecards.append( object );
		object->setProperty( "text", newCardText );
		object->setProperty( "title", newCardTitle );
		object->setProperty( "associationType", ( uint_fast8_t ) assocType);
		object->setProperty( "associatedID", associatedID );
		object->setProperty( "isDuplicate", false );
		
		if( maxIDWithinAssociatedThing.size() <= associatedID ) {
			maxIDWithinAssociatedThing.resize( associatedID + 1, 0 );
		}
		
		maxIDWithinAssociatedThing[ associatedID ] += 1;
		object->setProperty( "idWithinAssociatedThing", maxIDWithinAssociatedThing[ associatedID ] );
		
		if( associatedID < INT_MAX ) {
			associateNotecardWith( object, assocType, associatedID );
		}
		
		emit notecardsChanged();
	}
}

Q_INVOKABLE void NotecardManager::associateNotecardWith( QObject* notecard, associationType assocType, int associatedID ) {
	QQmlComponent component( engine, QUrl( QStringLiteral( "qrc:///TextNotecard.qml" ) ) );
	
	while( Q_UNLIKELY( component.status() == QQmlComponent::Loading ) ) {};
	
	if( Q_UNLIKELY( component.status() == QQmlComponent::Error ) ) {
		std::cerr << "Error creating component" << std::endl;
	} else {
		QObject* object2 = component.create();
		engine->setObjectOwnership( object2, QQmlEngine::CppOwnership );
		//object->setProperty( "charactersTab", charactersTab );
		//notecards.append( object2 );
		
		if( ( uint_fast8_t ) assocType >= notecardsWithAssociations.size() ) {
			notecardsWithAssociations.resize( ( uint_fast8_t ) assocType + 1 );
		}
		
		if( ( uint_fast8_t ) associatedID >= notecardsWithAssociations[ ( uint_fast8_t ) assocType ].size() ) {
			notecardsWithAssociations[ ( uint_fast8_t ) assocType ].resize( associatedID + 1 );
		}
		
		notecardsWithAssociations[ ( uint_fast8_t ) assocType ][ associatedID ].append( object2 );
		notecardsWithDuplicates.push_back( notecard );
		object2->setProperty( "text", notecard->property( "text" ) );
		object2->setProperty( "title", notecard->property( "title" ) );
		object2->setProperty( "associationType", ( uint_fast8_t ) assocType);
		object2->setProperty( "associatedID", associatedID );
		
		object2->setProperty( "isDuplicate", true );
		//maxIDWithinAssociatedThing[ associatedID ] += 1;
		object2->setProperty( "idWithinAssociatedThing", maxIDWithinAssociatedThing[ associatedID ] );
		
		emit notecardsChanged();
	}
}

Q_INVOKABLE QList< QObject* > NotecardManager::getAllNotecards() {
	QList< QObject* > results;
	
	for( decltype( notecards )::size_type i = 0; i < notecards.size(); ++i ) {
		
		//if( notecards[ i ]->property( "associationType" ) == CHARACTER && characterID == notecards[ i ]->property( "associatedID" ).toInt() ) {
			results.append( notecards[ i ] );
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
	
	if( Q_LIKELY( notecardsWithAssociations.size() > ( uint_fast8_t ) associationType::CHARACTER ) ) {
		if( notecardsWithAssociations[ ( uint_fast8_t ) associationType::CHARACTER ].size() > characterID ) {
			results = notecardsWithAssociations[ ( uint_fast8_t ) associationType::CHARACTER ][ characterID ];
		}
	}
	
	return results;
}

void NotecardManager::removeAllNotecards() {
	notecards.clear();
	notecardsWithAssociations.clear();
	emit notecardsChanged();
}

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

void NotecardManager::updateNotecard( QObject* origin ) {
	//std::cout << "updateNotecard() called" << std::endl;
	if( !origin->property( "isDuplicate" ).toBool() ) {
		if( Q_LIKELY( notecardsWithAssociations.size() > ( uint_fast8_t ) associationType::CHARACTER ) ) {
			uint_fast8_t characterID = origin->property( "associatedID" ).toUInt();
			if( notecardsWithAssociations[ ( uint_fast8_t ) associationType::CHARACTER ].size() > characterID ) {
				auto notecardsForCharacter = notecardsWithAssociations[ ( uint_fast8_t ) associationType::CHARACTER ][ characterID ];
				for( auto iterator = notecardsForCharacter.begin(); iterator != notecardsForCharacter.end(); iterator++ ) {
					auto notecard = *iterator;
					if( origin->property( "associatedID" ).toUInt() == notecard->property( "associatedID" ).toUInt() && origin->property( "associationType" ).toUInt() == notecard->property( "associationType" ).toUInt() && origin->property( "idWithinAssociatedThing" ).toUInt() == notecard->property( "idWithinAssociatedThing" ).toUInt() ) {
						notecard->setProperty( "title", origin->property( "title" ) );
						notecard->setProperty( "text", origin->property( "text" ) );
						notecard->setProperty( "color", origin->property( "color" ) );
					}
				}
			}
		}
	} else {
		for( auto iterator = notecardsWithDuplicates.begin(); iterator != notecardsWithDuplicates.end(); iterator++ ) {
			auto notecard = *iterator;
			if( origin->property( "associatedID" ).toUInt() == notecard->property( "associatedID" ).toUInt() && origin->property( "associationType" ).toUInt() == notecard->property( "associationType" ).toUInt() && origin->property( "idWithinAssociatedThing" ).toUInt() == notecard->property( "idWithinAssociatedThing" ).toUInt() ) {
				notecard->setProperty( "title", origin->property( "title" ) );
				notecard->setProperty( "text", origin->property( "text" ) );
				notecard->setProperty( "color", origin->property( "color" ) );
			}
		}
	}
}