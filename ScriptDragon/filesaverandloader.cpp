#include "filesaverandloader.h"
#include <QByteArray>
#include <QFile>
#include <QList>
#include <QJsonArray>
#include <QJsonDocument>
//This file MUST be stored as UTF-8 because of headerString

#include <QJsonObject>
#include <QJsonParseError>
#include <QSaveFile>
#include <QtGlobal>
#include <QUrl>
#include <iostream>
#include <string>
#include <QVariant>
#include <QQuickItem>

FileSaverAndLoader::FileSaverAndLoader( QObject *parent ) : QObject( parent ) {
	notecardManager = nullptr;
	scriptPage = nullptr;
	charactersPage = nullptr;
	storylinesPage = nullptr;
	headerString = QString::fromUtf8( u8"\U0001F409\U0001F432ScriptDragon\U0001F432\U0001F409\n" );
}

void FileSaverAndLoader::load( QUrl fileURL ) {
	QString fileName = fileURL.toLocalFile();
	
	QFile loadFile( fileName );
	if( Q_UNLIKELY( !loadFile.exists() ) ) {
		
		std::cerr << "Selected file \"" << fileName.toStdString() << "\" does not exist" << std::endl;
		
	} else if( Q_UNLIKELY( !loadFile.open( QIODevice::ReadOnly ) ) ) {
		
		std::cerr << "Selected file \"" << fileName.toStdString() << "\" could not be opened for reading" << std::endl;
		
	} else {
		
		QByteArray contents = loadFile.readAll();
		QByteArray header = headerString.toUtf8();
		
		if( Q_UNLIKELY( contents.isNull() || contents.isEmpty() ) ) {
			
			std::cerr << "Error reading file \"" << fileName.toStdString() << "\"" << std::endl;
			
		} else if( Q_UNLIKELY( !contents.startsWith( header ) ) ) {
			
			std::cerr << "Error reading file \"" << fileName.toStdString() << "\": Header not found" << std::endl;
			
		} else {
			
			contents.remove( 0, header.length() );
			
			QJsonParseError errorHolder;
			QJsonDocument jsonDoc = QJsonDocument::fromJson( contents, &errorHolder );
			
			if( errorHolder.error != QJsonParseError::NoError ) {
				
				std::cerr << "Error reading file \"" << fileName.toStdString() << "\": " << errorHolder.errorString().toStdString() << std::endl;
				
			} else if( !jsonDoc.isObject() ) {
				
				std::cerr << "Error reading file \"" << fileName.toStdString() << "\": Incorrect file format" << std::endl;
				
			} else {
				
				QJsonObject json = jsonDoc.object();
				
				//script
				if( Q_LIKELY( scriptPage != nullptr ) && Q_LIKELY( json.contains( "script" ) ) ) {
					
					QString scriptText = json.take( "script" ).toString();
					
					scriptPage->setProperty( "text", scriptText );
					/*auto testing = scriptPage->findChild<QTextArea *>("scriptTA");
					std::cout << 5 << std::endl;*/
					
				}
				
				//characters
				if( Q_LIKELY( charactersPage != nullptr ) && Q_LIKELY( json.contains( "characters" ) ) ) {
					
					QJsonArray characters = json.take( "characters" ).toArray();
					for( auto i = characters.begin(); i != characters.end(); ++i ) {
						
						QString characterName = (*i).toString();
						emit addCharacterName( characterName );
						
					}
					
				}
				
				//locations
				if( Q_LIKELY( locationsPage != nullptr ) && Q_LIKELY( json.contains( "locations" ) ) ) {
					
					QJsonArray locations = json.take( "locations" ).toArray();
					for( auto i = locations.begin(); i != locations.end(); ++i ) {
						
						QString locationName = (*i).toString();
						emit addLocationName( locationName );
						
					}
					
				}
				
				//storylines
				if( Q_LIKELY( storylinesPage != nullptr ) && Q_LIKELY( json.contains( "storylines" ) ) ) {
					
					QJsonArray storylines = json.take( "storylines" ).toArray();
					for( auto i = storylines.begin(); i != storylines.end(); ++i ) {
						
						QString storylineName = (*i).toString();
						emit addStorylineName( storylineName );
						
					}
					
				}
				
				//notecards
				if( Q_LIKELY( notecardManager != nullptr ) && Q_LIKELY( json.contains( "notecards" ) ) ) {
					
					QJsonArray notecards = json.take( "notecards" ).toArray();
					for( auto i = notecards.begin(); i != notecards.end(); ++i ) {
						
						QJsonObject notecard = (*i).toObject();
						NotecardManager::associationType associationType = (NotecardManager::associationType) ( (uint_fast8_t) notecard.take( "associationType" ).toInt() );
						int associatedID = notecard.take( "associatedID" ).toInt();
						QString color = notecard.take( "color" ).toString();
						QString title = notecard.take( "title" ).toString();
						QString text = notecard.take( "text" ).toString();
						int idWithinAssociatedThing = notecard.take( "idWithinAssociatedThing" ).toInt();
						
						notecardManager->addNotecard( title, text, associationType, associatedID, idWithinAssociatedThing, color );
					}
					
				}
			}
		}
	}
	
	emit fileLoaded( fileURL );
}

void FileSaverAndLoader::save( QUrl fileURL ) {
	
	QString fileName = fileURL.toLocalFile();
	if( !fileName.endsWith( ".scriptdragon", Qt::CaseInsensitive ) ) {
		
		fileName.append( ".scriptdragon" );
		
	}
	
	QSaveFile saveFile( fileName );
	std::cout << "saving to \"" << saveFile.fileName().toStdString().c_str() << "\"" << std::endl;
	
	saveFile.setDirectWriteFallback( true );
	if( Q_UNLIKELY( !saveFile.open( QIODevice::WriteOnly ) ) ) {
		
		std::cerr << "Couldn't open file for writing" << std::endl;
		saveFile.cancelWriting(); //I doubt this is needed since open() failed
		
	} else {
		
		{
			QJsonObject json;
			
			//script
			if( Q_LIKELY( scriptPage != nullptr ) ) {
				
				json.insert( "script", scriptPage->property( "text" ).toString() );
				
			}
			
			//notecards
			if( Q_LIKELY( notecardManager != nullptr ) ) {
				
				QJsonArray notecardArray;
				foreach( QObject* notecard, notecardManager->getAllNotecards() ) {
					
					QJsonObject cardJson;
					
					QList< QString > stringProperties( { "title", "text", "color" } );
					foreach( QString propertyName, stringProperties ) {
						
						cardJson.insert( propertyName, notecard->property( propertyName.toStdString().c_str() ).toString() );
						
					}
					QList< QString > intProperties( { "associationType", "associatedID", "idWithinAssociatedThing" } );
					foreach( QString propertyName, intProperties ) {
						
						cardJson.insert( propertyName, notecard->property( propertyName.toStdString().c_str() ).toInt() );
						
					}
					
					notecardArray.append( cardJson );
					
				}
				
				json.insert( "notecards", notecardArray );
				
			}
			
			//characters
			if( Q_LIKELY( charactersPage != nullptr ) ) {
				
				QJsonArray characterArray;
				auto characterView = charactersPage->findChild< QObject* >( "characterView" );
				auto children = characterView->children();
				
				for( uint_fast8_t i = 0; i < children.size(); ++i ) {
					QObject* child = children[ i ];
					QString name = child->property( "name" ).toString();
					characterArray.append( QJsonValue( name ) );
				}
				
				json.insert( "characters", characterArray );
				
			}
			
			//locations
			if( Q_LIKELY( locationsPage != nullptr ) ) {
				
				QJsonArray locationArray;
				auto locationView = locationsPage->findChild< QObject* >( "locationView" );
				auto children = locationView->children();
				
				for( uint_fast8_t i = 0; i < children.size(); ++i ) {
					QObject* child = children[ i ];
					QString name = child->property( "name" ).toString();
					locationArray.append( QJsonValue( name ) );
				}
				
				json.insert( "locations", locationArray );
				
			}
			
			//storylines
			if( Q_LIKELY( storylinesPage != nullptr ) ) {
				
				QJsonArray storylineArray;
				auto storylineView = storylinesPage->findChild< QObject* >( "storylineView" );
				auto children = storylineView->children();
				
				for( uint_fast8_t i = 0; i < children.size(); ++i ) {
					QObject* child = children[ i ];
					QString name = child->property( "name" ).toString();
					storylineArray.append( QJsonValue( name ) );
				}
				
				json.insert( "storylines", storylineArray );
				
			}
			
			QByteArray fileContent = headerString.toUtf8();
			QJsonDocument jsonDoc( json );
			fileContent.append( jsonDoc.toJson( QJsonDocument::Indented ) );
			saveFile.write( fileContent );
			
		}
		
		//..then when we're done writing data, we commit.
		if( Q_UNLIKELY( !saveFile.commit() ) ) {
			
			std::cerr << "Error saving file" << std::endl;
			
		}
	}
	
	emit fileSaved( fileURL );
	
}

void FileSaverAndLoader::setCharactersPage( QObject* newCharactersPage ) {
	
	charactersPage = newCharactersPage;
	
}

void FileSaverAndLoader::setLocationsPage( QObject* newLocationsPage ) {
	
	locationsPage = newLocationsPage;
	
}

void FileSaverAndLoader::setNotecardManager( NotecardManager* newNM ) {
	
	notecardManager = newNM;
	
}

void FileSaverAndLoader::setScriptPage( QObject* newScriptPage ) {
	
	scriptPage = newScriptPage;
	
}

void FileSaverAndLoader::setStorylinesPage( QObject* newStorylinesPage ) {
	
	storylinesPage = newStorylinesPage;
	
}