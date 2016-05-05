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
		
		
		if( associatedID < INT_MAX ) {
			if( maxIDWithinAssociatedThing.size() <= associatedID ) {
				maxIDWithinAssociatedThing.resize( associatedID + 1, 0 );
			}
			
			maxIDWithinAssociatedThing[ associatedID ] += 1;
			object->setProperty( "idWithinAssociatedThing", maxIDWithinAssociatedThing[ associatedID ] );
			associateNotecardWith( object, assocType, associatedID );
		} else {
			object->setProperty( "idWithinAssociatedThing", 0 );
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

void NotecardManager::copyNotecardData(QObject* origin, QObject* destination) {
	destination->setProperty( "title", origin->property( "title" ) );
	destination->setProperty( "text", origin->property( "text" ) );
	destination->setProperty( "color", origin->property( "color" ) );
	destination->setProperty( "associatedID", origin->property( "associatedID" ) );
	//destination->setProperty( "associatedText", origin->property( "associatedText" ) );
	destination->setProperty( "associationType", origin->property( "associationType" ) );
	//destination->update();
}

Q_INVOKABLE QList< QObject* > NotecardManager::getAllNotecards() {
	QList< QObject* > results;
	
	for( decltype( notecards.size() ) i = 0; i < notecards.size(); ++i ) {
		
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

void NotecardManager::removeAssociation( QObject* notecard ) {
	
	if( !notecard->property( "isDuplicate" ).toBool() ) {
		uint_fast8_t assocType = notecard->property( "associationType" ).toUInt();
		if( Q_LIKELY( notecardsWithAssociations.size() > assocType ) ) {
			uint_fast8_t associatedID = notecard->property( "associatedID" ).toUInt();
			if( notecardsWithAssociations[ assocType ].size() > associatedID ) {
				auto notecardsForCharacter = notecardsWithAssociations[ assocType ][ associatedID ];
				
				switch( assocType ) {
					case ( uint_fast8_t ) associationType::CHARACTER: {
						for( auto iterator = notecardsForCharacter.begin(); iterator != notecardsForCharacter.end(); iterator++ ) {
							auto otherNotecard = *iterator;
							if( associatedID == otherNotecard->property( "associatedID" ).toUInt() && assocType == otherNotecard->property( "associationType" ).toUInt() && notecard->property( "idWithinAssociatedThing" ).toUInt() == otherNotecard->property( "idWithinAssociatedThing" ).toUInt() ) {
								
								std::cout << "Before: " << notecard->property( "associationType" ).toUInt() << std::endl;
								
								notecard->setProperty( "associationType", ( uint_fast8_t ) associationType::NONE );
								
								std::cout << "After: " << notecard->property( "associationType" ).toUInt() << std::endl;
								
								std::cout << "Before: " << notecard->property( "associatedID" ).toUInt() << std::endl;
								notecard->setProperty( "associatedID", INT_MAX );
								std::cout << "After: " << notecard->property( "associatedID" ).toUInt() << std::endl;
								notecard->setProperty( "idWithinAssociatedThing", INT_MAX );
								
								copyNotecardData( notecard, otherNotecard );
								
								notecardsForCharacter.removeAll( otherNotecard );
								notecardsForCharacter.removeAll( notecard );
							}
						}
						break;
					}
				}
				
			}
		}
		
		notecard->deleteLater();
	} else {
		
		uint_fast8_t assocType = notecard->property( "associationType" ).toUInt();
		uint_fast8_t associatedID = notecard->property( "associatedID" ).toUInt();
		
		for( auto iterator = notecardsWithDuplicates.begin(); !notecardsWithDuplicates.empty() && iterator != notecardsWithDuplicates.end(); iterator++ ) {
			auto otherNotecard = *iterator;
			if( associatedID == otherNotecard->property( "associatedID" ).toUInt() && assocType == otherNotecard->property( "associationType" ).toUInt() && notecard->property( "idWithinAssociatedThing" ).toUInt() == otherNotecard->property( "idWithinAssociatedThing" ).toUInt() ) {
				
				
				otherNotecard->setProperty( "associationType", ( uint_fast8_t ) associationType::NONE );
				
				otherNotecard->setProperty( "associatedID", INT_MAX );
				
				otherNotecard->setProperty( "idWithinAssociatedThing", INT_MAX );
				
				notecardsWithDuplicates.erase( iterator );
				iterator = notecardsWithDuplicates.begin();
				notecardsWithAssociations[ assocType ][ associatedID ].removeAll( otherNotecard );
			}
		}
		
		
		std::cout << "Before: " << notecard->property( "associationType" ).toUInt() << std::endl;
		
		notecard->setProperty( "associationType", ( uint_fast8_t ) associationType::NONE );
		
		std::cout << "After: " << notecard->property( "associationType" ).toUInt() << std::endl;
		
		std::cout << "Before: " << notecard->property( "associatedID" ).toUInt() << std::endl;
		notecard->setProperty( "associatedID", INT_MAX );
		std::cout << "After: " << notecard->property( "associatedID" ).toUInt() << std::endl;
		notecard->setProperty( "idWithinAssociatedThing", INT_MAX );
		notecardsWithAssociations[ assocType ][ associatedID ].removeAll( notecard );
	}
	
	emit notecardsChanged();
}

Q_INVOKABLE void NotecardManager::removeNotecard( QObject* toRemove ) {
	notecards.removeAll( toRemove );
	
	if( !toRemove->property( "isDuplicate" ).toBool() ) {
		std::cout << "This is not a duplicate" << std::endl;
		uint_fast8_t assocType = toRemove->property( "associationType" ).toUInt();
		if( assocType < notecardsWithAssociations.size() ) {
			
			uint_fast8_t associatedID = toRemove->property( "associatedID" ).toUInt();
			if( associatedID < notecardsWithAssociations[ assocType ].size() ) {
				//notecardsWithAssociations[ ( uint_fast8_t ) assocType ][ associatedID ].append( object2 );
				for( auto i = notecardsWithAssociations[ assocType ][ associatedID ].begin(); i != notecardsWithAssociations[ assocType ][ associatedID ].end(); ++i ) {
					QObject* otherNotecard = *i;
					if( otherNotecard->property( "associationType" ).toUInt() == assocType && otherNotecard->property( "associatedID" ).toUInt() == associatedID && otherNotecard->property( "idWithinAssociatedThing" ).toUInt() == toRemove->property( "idWithinAssociatedThing" ).toUInt() ) {
						if( otherNotecard->property( "isDuplicate" ).toBool() ) {
							std::cout << "deleting the duplicate" << std::endl;
						}
						notecardsWithAssociations[ assocType ][ associatedID ].erase( i );
					}
				}
			}
			
		}
		
		for( decltype( notecardsWithDuplicates.size() ) i = 0; i < notecardsWithDuplicates.size(); ++i ) {
			if( toRemove == notecardsWithDuplicates[ i ] ) {
				notecardsWithDuplicates.erase( notecardsWithDuplicates.begin() + i );
			}
		}
	} else {
		std::cout << "This is a duplicate" << std::endl;
		uint_fast8_t associatedID = toRemove->property( "associatedID" ).toUInt();
		uint_fast8_t assocType = toRemove->property( "associationType" ).toUInt();
		uint_fast8_t idWithin = toRemove->property( "idWithinAssociatedThing" ).toUInt();
		
		for( decltype( notecardsWithDuplicates.size() ) i = 0; i < notecardsWithDuplicates.size(); ++i ) {
			
			auto notecard = notecardsWithDuplicates[ i ];
			if( notecard->property( "associatedID" ).toUInt() == associatedID && notecard->property( "associationType" ).toUInt() == assocType && notecard->property( "idWithinAssociatedThing" ).toUInt() == idWithin ) {
				std::cout << "Found the original" << std::endl;
				removeNotecard( notecard );
			}
			
		}
	}
	
	toRemove->deleteLater();
	
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
		uint_fast8_t associationType = origin->property( "associationType" ).toUInt();
		if( Q_LIKELY( notecardsWithAssociations.size() > associationType ) ) {
			uint_fast8_t associatedID = origin->property( "associatedID" ).toUInt();
			if( notecardsWithAssociations[ associationType ].size() > associatedID ) {
				auto notecardsForCharacter = notecardsWithAssociations[ associationType ][ associatedID ];
				
				switch( associationType ) {
					case ( uint_fast8_t ) NotecardManager::associationType::CHARACTER: {
						for( auto iterator = notecardsForCharacter.begin(); iterator != notecardsForCharacter.end(); iterator++ ) {
							auto notecard = *iterator;
							if( associatedID == notecard->property( "associatedID" ).toUInt() && associationType == notecard->property( "associationType" ).toUInt() && origin->property( "idWithinAssociatedThing" ).toUInt() == notecard->property( "idWithinAssociatedThing" ).toUInt() ) {
								copyNotecardData( origin, notecard );
							}
						}
						break;
					}
				}
				
			}
		}
	} else {
		for( auto iterator = notecardsWithDuplicates.begin(); iterator != notecardsWithDuplicates.end(); iterator++ ) {
			auto notecard = *iterator;
			if( origin->property( "associatedID" ).toUInt() == notecard->property( "associatedID" ).toUInt() && origin->property( "associationType" ).toUInt() == notecard->property( "associationType" ).toUInt() && origin->property( "idWithinAssociatedThing" ).toUInt() == notecard->property( "idWithinAssociatedThing" ).toUInt() ) {
				copyNotecardData( origin, notecard );
			}
		}
	}
	
	emit notecardsChanged();
}