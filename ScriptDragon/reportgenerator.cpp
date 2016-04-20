#include <iostream>
#include "reportgenerator.h"
#include <QList>
#include <QString>
#include <QTextEdit>

ReportGenerator::ReportGenerator( QQmlEngine* newEngine, QObject *parent ) : QObject(parent) {
	
}

QString ReportGenerator::generateReport( int numerator, int denominator ) {
	QTextDocument report;
	//auto script = scriptPage->property( "text" )->textDocument();
	
	switch( denominator ) {
		case (uint_fast8_t) ScriptFormatter::paragraphType::SCENE: {
			switch( numerator ) {
				case (uint_fast8_t) ScriptFormatter::paragraphType::CHARACTER: {
					QList<QString> charactersFound;
					
					QTextCursor cursor( report.firstBlock() );
					uint_fast16_t sceneNumber = 0;
					auto doc = script->textDocument();
					for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
						if( block == doc->firstBlock() || block.userState() == (uint_fast8_t) ScriptFormatter::paragraphType::SCENE ) {
							sceneNumber += 1;
							cursor.insertBlock();
							cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() );
							charactersFound.clear();
						}
						
						if( block.userState() == (uint_fast8_t) ScriptFormatter::paragraphType::CHARACTER ) {
							if( !charactersFound.contains( block.text() ) ) {
								charactersFound.append( block.text() );
								cursor.insertText( block.text() + ", " );
							}
						}
					}
					
					break;
				}
			}
			
			
			
			break;
		}
	}
	
	
	std::cout << report.toHtml().toStdString() << std::endl;
	return report.toHtml();
}

Q_INVOKABLE void ReportGenerator::setScript( QQuickTextDocument* newScript ) {
	script = newScript;
}
