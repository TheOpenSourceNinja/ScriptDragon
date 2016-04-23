#include "filesaverandloader.h"
#include <QByteArray>
#include <QFile>
#include <QList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSaveFile>
#include <QtGlobal>
#include <iostream>
#include <string>

FileSaverAndLoader::FileSaverAndLoader( QObject *parent ) : QObject( parent ) {
	notecardManager = nullptr;
	scriptPage = nullptr;
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
		
		if( Q_UNLIKELY( contents.isNull() || contents.isEmpty() ) ) {
			std::cerr << "Error reading file \"" << fileName.toStdString() << "\"" << std::endl;
		} else {
			QJsonParseError errorHolder;
			QJsonDocument jsonDoc = QJsonDocument::fromJson( contents, &errorHolder );
			
			if( errorHolder.error != QJsonParseError::NoError ) {
				std::cerr << "Error reading file \"" << fileName.toStdString() << "\": " << errorHolder.errorString().toStdString() << std::endl;
			} else if( !jsonDoc.isObject() ) {
				std::cerr << "Error reading file \"" << fileName.toStdString() << "\": Incorrect file format" << std::endl;
			} else {
				QJsonObject json = jsonDoc.object();
				
				if( Q_LIKELY( scriptPage != nullptr ) ) {
					QString scriptText = json.take( "script" ).toString();
					std::cout << "Script text: \"" << scriptText.toStdString() << "\"" << std::endl;
					
					scriptPage->setProperty( "text", scriptText );
					/*auto testing = scriptPage->findChild<QTextArea *>("scriptTA");
					std::cout << 5 << std::endl;*/
				}
				
				if( Q_LIKELY( notecardManager != nullptr ) ) {
					//TODO: Stuff.
				}
			}
		}
	}
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
			
			if( Q_LIKELY( scriptPage != nullptr ) ) {
				json.insert( "script", scriptPage->property("text").toString() );
			}
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
			
			QJsonDocument jsonDoc( json );
			saveFile.write( jsonDoc.toJson( QJsonDocument::Indented ) );
		}
		
		//..then when we're done writing data, we commit.
		if( Q_UNLIKELY( !saveFile.commit() ) ) {
			std::cerr << "Error saving file" << std::endl;
		}
	}
}

void FileSaverAndLoader::setNotecardManager(NotecardManager* newNM) {
	notecardManager = newNM;
}

void FileSaverAndLoader::setScriptPage(QObject* newScriptPage) {
	scriptPage = newScriptPage;
}

