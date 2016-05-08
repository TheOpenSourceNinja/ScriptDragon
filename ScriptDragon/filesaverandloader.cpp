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
	headerString = "🐉🐲ScriptDragon🐲🐉\n";
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
		
		if( Q_UNLIKELY( contents.isNull() || contents.isEmpty() || !contents.startsWith( header ) ) ) {
			
			std::cerr << "Error reading file \"" << fileName.toStdString() << "\"" << std::endl;
			
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
				
				if( Q_LIKELY( scriptPage != nullptr ) && Q_LIKELY( json.contains( "script" ) ) ) {
					
					QString scriptText = json.take( "script" ).toString();
					
					scriptPage->setProperty( "text", scriptText );
					/*auto testing = scriptPage->findChild<QTextArea *>("scriptTA");
					std::cout << 5 << std::endl;*/
					
				}
				
				if( Q_LIKELY( charactersPage != nullptr ) && Q_LIKELY( json.contains( "characters" ) ) ) {
					
					QJsonArray characters = json.take( "characters" ).toArray();
					for( auto i = characters.begin(); i != characters.end(); ++i ) {
						
						QString characterName = (*i).toString();
						emit addCharacterName( characterName );
						
					}
					
				}
				
				if( Q_LIKELY( locationsPage != nullptr ) && Q_LIKELY( json.contains( "locations" ) ) ) {
					
					//TODO: Stuff.
					
				}
				
				if( Q_LIKELY( notecardManager != nullptr ) && Q_LIKELY( json.contains( "notecards" ) ) ) {
					
					//TODO: Stuff.
					
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
		
		//TODO: write a bunch of data here...
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
					
					QList< QString > properties( { "title", "text", "color", "associationType", "associatedID", "associatedText" } );
					foreach( QString propertyName, properties ) {
						
						cardJson.insert( propertyName, notecard->property( propertyName.toStdString().c_str() ).toString() );
						
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

