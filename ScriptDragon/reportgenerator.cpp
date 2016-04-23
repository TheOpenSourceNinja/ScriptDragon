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
			if( numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			cursor.insertBlock();
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
			
		}
	}
}

void ReportGenerator::charactersPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			cursor.insertBlock();
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
			}
			
		}
	}
	
	std::sort( numeratorsFound.begin(), numeratorsFound.end() );
}

void ReportGenerator::dialogsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			cursor.insertBlock();
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
			
		}
	}
}

void ReportGenerator::parentheticalsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			cursor.insertBlock();
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
			
		}
	}
}

void ReportGenerator::transitionsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			cursor.insertBlock();
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
			
		}
	}
}

void ReportGenerator::shotsPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			cursor.insertBlock();
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
			
		}
	}
}

void ReportGenerator::actBreaksPerScene( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	uint_fast16_t sceneNumber = 0;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			if( numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			sceneNumber += 1;
			cursor.insertBlock();
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + ": " ).c_str() ); //TODO: Translate
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
			
			if( !numeratorsFound.contains( block.text() ) ) {
				numeratorsFound.append( block.text() );
				cursor.insertText( block.text() + ", " );
			}
			
		}
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
