#include <algorithm>
#include <iostream>
#include "reportgenerator.h"
#include <QList>
#include <QString>
#include <QTextEdit>

ReportGenerator::ReportGenerator( QQmlEngine* newEngine, QObject *parent ) : QObject(parent) {
	
}

void ReportGenerator::actionsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( sceneNumber != 0 && numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
			
		}
	}
	
	if( numeratorsFound.empty() ) {
		cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::charactersPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( block != doc->firstBlock() && numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			if( !numeratorsFound.empty() ) {
				std::sort( numeratorsFound.begin(), numeratorsFound.end() );
				for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
					cursor.insertText( *iterator + ", " );
				}
			}
			
			sceneNumber += 1;
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			//cursor.insertText( block.text() + ", " );
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
			}
			
		}
	}
	
	std::sort( numeratorsFound.begin(), numeratorsFound.end() );
	for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
		cursor.insertText( *iterator + ", " );
	}
}

void ReportGenerator::dialogsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	bool foundAnything = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( block != doc->firstBlock() && !foundAnything ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			foundAnything = false;
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
			foundAnything = true;
			
			QTextBlock previous = block.previous();
			while( previous.userState() != ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER && previous.length() > 0 ) {
				previous = previous.previous();
			}
			cursor.insertText( previous.text() + ": " );
			cursor.insertText( block.text() + ", " );
			
		}
	}
	
	if( !foundAnything ) {
		cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::parentheticalsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	bool foundAnything = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( block != doc->firstBlock() && !foundAnything ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			foundAnything = false;
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
			foundAnything = true;
			
			QTextBlock previous = block.previous();
			while( previous.userState() != ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER && previous.length() > 0 ) {
				previous = previous.previous();
			}
			cursor.insertText( previous.text() + ": " );
			cursor.insertText( block.text() + ", " );
			
		}
	}
	
	if( !foundAnything ) {
		cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::transitionsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	bool foundAnything = false;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( block != doc->firstBlock() && !foundAnything ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + "🡒" + std::to_string( sceneNumber + 1 ) + ": " ).c_str() ); //TODO: Translate
			foundAnything = false;
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			
			foundAnything = true;
			cursor.insertText( block.text() );
			
		}
	}
	
	if( !foundAnything ) {
		cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::shotsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	bool foundAnything = false;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( block != doc->firstBlock() && !foundAnything ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			foundAnything = false;
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			
			foundAnything = true;
			cursor.insertText( block.text() + ", " );
			
		}
	}
	
	if( !foundAnything ) {
		cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::actBreaksPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	bool foundAnything = false;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( block != doc->firstBlock() && !foundAnything ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			foundAnything = false;
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
			
			foundAnything = true;
			cursor.insertText( block.text() );
			
		}
	}
	
	if( !foundAnything ) {
		cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::dialogsPerCharacter( QTextDocument* report ) {
	std::cout << report->toPlainText().toStdString() << std::endl;
	QString temp = report->toPlainText();
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	QString character;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			character = block.text();
			cursor.insertBlock();
			cursor.insertText( character + ": " );
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
			
		}
		temp = report->toPlainText();
	}
}

QString ReportGenerator::generateReport( int numerator, int denominator ) {
	QTextDocument report;
	
	if( numerator == denominator ) {
		return report.toHtml();
	}
	
	//auto script = scriptPage->property( "text" )->textDocument();
	
	switch( numerator ) {
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					actionsPerScene( &report );
					break;
				}
			}
			break;
		}
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					charactersPerScene( &report );
					break;
				}
			}
			break;
		}
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					dialogsPerScene( &report );
					break;
				}
				case( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					dialogsPerCharacter( &report );
					break;
				}
			}
			break;
		}
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					parentheticalsPerScene( &report );
					break;
				}
			}
			break;
		}
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					transitionsPerScene( &report );
					break;
				}
			}
			break;
		}
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					shotsPerScene( &report );
					break;
				}
			}
			break;
		}
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					shotsPerScene( &report );
					break;
				}
			}
			break;
		}
	}
	
	/*QList<QString> numeratorsFound;
	
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
	}*/
	
	
	std::cout << report.toHtml().toStdString() << std::endl;
	return report.toHtml();
}

Q_INVOKABLE void ReportGenerator::setScript( QQuickTextDocument* newScript ) {
	script = newScript;
}
