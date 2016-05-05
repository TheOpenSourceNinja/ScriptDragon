#include <algorithm>
#include <iostream>
#include "reportgenerator.h"
#include <QList>
#include <QString>
#include <QTextEdit>

ReportGenerator::ReportGenerator( QQmlEngine* newEngine, QObject *parent ) : QObject(parent) {
	script = nullptr;
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
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + "(" + block.text().toStdString() + "): " ).c_str() ); //TODO: Translate
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
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + "(" + block.text().toStdString() + "): " ).c_str() ); //TODO: Translate
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
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + "(" + block.text().toStdString() + "): " ).c_str() ); //TODO: Translate
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
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + "(" + block.text().toStdString() + "): " ).c_str() ); //TODO: Translate
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
			
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + "(" + block.text().toStdString() + "): " ).c_str() ); //TODO: Translate
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
			cursor.insertText( std::string( "Scene #" + std::to_string( sceneNumber ) + "(" + block.text().toStdString() + "): " ).c_str() ); //TODO: Translate
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

void ReportGenerator::scenesPerAction( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	uint_fast8_t sceneNumber = 0;
	auto doc = script->textDocument();
	bool foundAnything = false;
	QString scene = "";
	bool firstActionFound = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			if( Q_LIKELY( firstActionFound ) ) {
				cursor.insertBlock();
			} else {
				firstActionFound = true;
			}
			cursor.insertText( block.text() + ": " + scene );
			foundAnything = true;
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			
			sceneNumber += 1;
			foundAnything = false;
			scene = QString( std::string( "Scene #" + std::to_string( sceneNumber ) + " (" + block.text().toStdString() + ")" ).c_str() ); //TODO: Translate
			
		}
	}
	
	if( !foundAnything ) {
		//cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::charactersPerAction( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block == doc->firstBlock() || block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			if( block != doc->firstBlock() && numeratorsFound.empty() ) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			if( !numeratorsFound.empty() ) {
				std::sort( numeratorsFound.begin(), numeratorsFound.end() );
				for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
					cursor.insertText( *iterator + ", " );
				}
			}
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			
			cursor.insertText( block.text() + ": " );
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

void ReportGenerator::dialogsPerAction( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	bool firstActionFound = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			
			if( !numeratorsFound.empty() ) {
				std::sort( numeratorsFound.begin(), numeratorsFound.end() );
				for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
					cursor.insertText( *iterator + ", " );
				}
			}
			
			if( Q_LIKELY( firstActionFound ) ) {
				cursor.insertBlock();
			} else {
				firstActionFound = true;
			}
			
			cursor.insertText( block.text() + ": " );
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
			auto speaker = block.previous();
			while( Q_UNLIKELY( speaker.userState() != ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) && Q_LIKELY( speaker.length() > 0 ) ) {
				speaker = speaker.previous();
			}
			numeratorsFound.append( speaker.text() + ": " + block.text() );
			
		}
	}
	
	std::sort( numeratorsFound.begin(), numeratorsFound.end() );
	for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
		cursor.insertText( *iterator + ", " );
	}
}

void ReportGenerator::parentheticalsPerAction( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	bool firstActionFound = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			
			if( !numeratorsFound.empty() ) {
				std::sort( numeratorsFound.begin(), numeratorsFound.end() );
				for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
					cursor.insertText( *iterator + ", " );
				}
			}
			
			if( Q_LIKELY( firstActionFound ) ) {
				cursor.insertBlock();
			} else {
				firstActionFound = true;
			}
			
			cursor.insertText( block.text() + ": " );
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
			auto speaker = block.previous();
			while( Q_UNLIKELY( speaker.userState() != ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) && Q_LIKELY( speaker.length() > 0 ) ) {
				speaker = speaker.previous();
			}
			numeratorsFound.append( speaker.text() + ": " + block.text() );
			
		}
	}
	
	std::sort( numeratorsFound.begin(), numeratorsFound.end() );
	for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
		cursor.insertText( *iterator + ", " );
	}
}

void ReportGenerator::transitionsPerAction( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QList<QString> numeratorsFound;
	auto doc = script->textDocument();
	bool firstActionFound = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			
			if( Q_LIKELY( firstActionFound ) ) {
				if( Q_LIKELY( numeratorsFound.empty() ) ) {
					cursor.insertText( "None found" ); //TODO: Translate.
				} else {
					std::sort( numeratorsFound.begin(), numeratorsFound.end() );
					for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
						cursor.insertText( *iterator + ", " );
					}
				}
				cursor.insertBlock();
			} else {
				firstActionFound = true;
			}
			
			cursor.insertText( block.text() + ": " );
			numeratorsFound.clear();
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			auto previousScene = block.previous();
			while( Q_UNLIKELY( previousScene.userState() != ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) && Q_LIKELY( previousScene.length() > 0 ) ) {
				previousScene = previousScene.previous();
			}
			auto nextScene = block.previous();
			while( Q_UNLIKELY( nextScene.userState() != ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) && Q_LIKELY( nextScene.length() > 0 ) ) {
				nextScene = nextScene.previous();
			}
			numeratorsFound.append( previousScene.text() + "🡒" + nextScene.text() + ": " + block.text() );
			
		}
	}
	
	if( Q_LIKELY( numeratorsFound.empty() ) ) {
		cursor.insertText( "None found" ); //TODO: Translate.
	} else {
		std::sort( numeratorsFound.begin(), numeratorsFound.end() );
		for( auto iterator = numeratorsFound.begin(); iterator != numeratorsFound.end(); iterator++ ) {
			cursor.insertText( *iterator + ", " );
		}
	}
}

void ReportGenerator::shotsPerAction( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	bool firstActionFound = false;
	QString shot = "";
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			
			if( Q_LIKELY( firstActionFound ) ) {
				cursor.insertBlock();
			} else {
				firstActionFound = true;
			}
			
			cursor.insertText( block.text() + ": " );
			
			if( Q_UNLIKELY( shot.length() == 0 ) ) {
				cursor.insertText( "None found" ); //TODO: Translate.
			} else {
				cursor.insertText( shot );
			}
			
			shot = "";
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			
			shot = block.text();
			
		}
	}
}

void ReportGenerator::actBreaksPerAction( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	bool firstActionFound = false;
	bool anythingFound = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			
			if( Q_LIKELY( firstActionFound ) ) {
				if( Q_LIKELY( !anythingFound ) ) {
					cursor.insertText( "None found" ); //TODO: Translate.
				}
				cursor.insertBlock();
			} else {
				firstActionFound = true;
			}
			
			cursor.insertText( block.text() + ": " );
			
			
			
			anythingFound = false;
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
			
			anythingFound = true;
			cursor.insertText( block.text() );
			
		}
	}
	
	if( Q_LIKELY( !anythingFound ) ) {
		cursor.insertText( "None found" ); //TODO: Translate.
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
					charactersPerAction( &report );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
					dialogsPerAction( &report );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
					parentheticalsPerAction( &report );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
					transitionsPerAction( &report );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
					shotsPerAction( &report );
					break;
				}
			}
			break;
		}
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					actBreaksPerScene( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
					actBreaksPerAction( &report );
					break;
				}
			}
			break;
		}
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
					scenesPerAction( &report );
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
	
	return report.toHtml();
}

Q_INVOKABLE void ReportGenerator::setScript( QQuickTextDocument* newScript ) {
	script = newScript;
}
