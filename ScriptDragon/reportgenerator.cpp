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
	
	
	QList<QString> numeratorsFound;
	
	QTextCursor cursor( report.firstBlock() );
	uint_fast16_t sceneNumber = 0;
	auto doc = script->textDocument();
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		switch( denominator ) {
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
				if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) denominator ) {
					if( numeratorsFound.empty() ) {
						cursor.insertText( "None found" ); //TODO: Translate
					}
					sceneNumber += 1;
					cursor.insertBlock();
					cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
					numeratorsFound.clear();
				}
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
				if( block.userState() == ( uint_fast8_t ) denominator ) {
					if( numeratorsFound.empty() ) {
						cursor.insertText( "None found" ); //TODO: Translate
					}
					cursor.insertBlock();
					cursor.insertText( block.text() + ": " );
					numeratorsFound.clear();
				}
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
				if( block.userState() == ( uint_fast8_t ) denominator ) {
					if( numeratorsFound.empty() ) {
						cursor.insertText( "None found" ); //TODO: Translate
					}
					cursor.insertBlock();
					cursor.insertText( block.text() + ": " );
					numeratorsFound.clear();
				}
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG: {
				if( block.userState() == ( uint_fast8_t ) denominator ) {
					if( numeratorsFound.empty() ) {
						cursor.insertText( "None found" ); //TODO: Translate
					}
					cursor.insertBlock();
					cursor.insertText( block.text() + ": " );
					numeratorsFound.clear();
				}
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
				if( block.userState() == ( uint_fast8_t ) denominator ) {
					if( numeratorsFound.empty() ) {
						cursor.insertText( "None found" ); //TODO: Translate
					}
					cursor.insertBlock();
					cursor.insertText( block.text() + ": " );
					numeratorsFound.clear();
				}
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
				if( block.userState() == ( uint_fast8_t ) denominator ) {
					if( numeratorsFound.empty() ) {
						cursor.insertText( "None found" ); //TODO: Translate
					}
					cursor.insertBlock();
					cursor.insertText( block.text() + ": " );
					numeratorsFound.clear();
				}
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
				if( block.userState() == ( uint_fast8_t ) denominator ) {
					if( numeratorsFound.empty() ) {
						cursor.insertText( "None found" ); //TODO: Translate
					}
					cursor.insertBlock();
					cursor.insertText( block.text() + ": " );
					numeratorsFound.clear();
				}
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK: {
				if( block.userState() == ( uint_fast8_t ) denominator ) {
					if( numeratorsFound.empty() ) {
						cursor.insertText( "None found" ); //TODO: Translate
					}
					cursor.insertBlock();
					cursor.insertText( block.text() + ": " );
					numeratorsFound.clear();
				}
				break;
			}
		}
		
		if( block.userState() == numerator ) {
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
		}
	}
	
	
	std::cout << report.toHtml().toStdString() << std::endl;
	return report.toHtml();
}

Q_INVOKABLE void ReportGenerator::setScript( QQuickTextDocument* newScript ) {
	script = newScript;
}
