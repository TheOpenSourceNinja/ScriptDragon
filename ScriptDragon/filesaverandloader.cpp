#include "filesaverandloader.h"
#include <QList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <iostream>
#include <string>

FileSaverAndLoader::FileSaverAndLoader(QObject *parent) : QObject(parent) {
	notecardManager = nullptr;
	scriptPage = nullptr;
}

void FileSaverAndLoader::save( QUrl fileURL ) {
	QString fileName = fileURL.toLocalFile();
	if( !fileName.endsWith( ".scriptdragon", Qt::CaseInsensitive ) ) {
		fileName.append( ".scriptdragon" );
	}
	
	QSaveFile saveFile( fileName );
	std::cout << "saving to \"" << saveFile.fileName().toStdString().c_str() << "\"" << std::endl;
	
	saveFile.setDirectWriteFallback( true );
	if( !saveFile.open( QIODevice::WriteOnly ) ) {
		std::cerr << "Couldn't open file for writing" << std::endl;
	}
	
	//TODO: write a bunch of data here...
	{
		QJsonObject json;
		
		if( scriptPage != nullptr ) {
			json.insert( "script", scriptPage->property("text").toString() );
		}
		if( notecardManager != nullptr ) {
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
		saveFile.write( jsonDoc.toJson() );
	}
	
	//..then when we're done writing data, we commit.
	if( !saveFile.commit() ) {
		std::cerr << "Error saving file" << std::endl;
	}
}

void FileSaverAndLoader::setNotecardManager(NotecardManager* newNM) {
	notecardManager = newNM;
}

void FileSaverAndLoader::setScriptPage(QObject* newScriptPage) {
	scriptPage = newScriptPage;
}

