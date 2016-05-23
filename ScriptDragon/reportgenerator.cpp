#include <algorithm>
#include <iostream>
#include "reportgenerator.h"
#include <QList>
#include <QString>
#include <QTextEdit>
#include <QTextTable>

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

void ReportGenerator::scenesPerCharacter( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	
	QStringList characters;
	QVector< QStringList > scenes;
	QString currentScene = "Unnamed first scene"; //TODO: Translate.
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			
			if( !characters.contains( block.text(), Qt::CaseInsensitive ) ) {
				characters.append( block.text() );
			}
			
			if( scenes.length() <= characters.indexOf( block.text() ) ) {
				scenes.resize( characters.indexOf( block.text() ) + 1 );
			}
			
			scenes[ characters.indexOf( block.text() ) ].append( currentScene );
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			
			currentScene = block.text();
			
		}
		
	}
	
	//TODO: Can we sort characters without messing up scenes?
	
	for( decltype( characters.size() ) c = 0; c < characters.size(); ++c ) {
		
		cursor.insertText( characters[ c ] + ": " );
		
		for( decltype( scenes[ c ].size() ) s = 0; s < scenes[ c ].size(); ++s ) {
			cursor.insertText( scenes[ c ][ s ] + ", " );
		}
		
		cursor.insertBlock();
		
	}
}

void ReportGenerator::actionsPerCharacter( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	
	QStringList characters;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			
			if( !characters.contains( block.text(), Qt::CaseInsensitive ) ) {
				characters.append( block.text() );
			}
			
		}
		
	}
	
	characters.sort();
	
	for( decltype( characters.size() ) c = 0; c < characters.size(); ++c ) {
		
		cursor.insertText( characters[ c ] + ": " );
		uint_fast8_t sceneNumber = 0;
		
		for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
			if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE || block == doc->firstBlock() ) {
				sceneNumber += 1;
			}
			
			if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
				
				if( block.text().contains( characters[ c ], Qt::CaseInsensitive ) ) {
					
					cursor.insertText( QString( "Scene #" ) + std::to_string( sceneNumber ).c_str() + ": " + block.text() + ", " ); //TODO: Translate.
					
				}
				
			}
			
		}
		
		cursor.insertBlock();
		
	}
}

void ReportGenerator::dialogsPerCharacter( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	
	QStringList characters;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			
			if( !characters.contains( block.text(), Qt::CaseInsensitive ) ) {
				
				characters.append( block.text() );
				
			}
			
		}
		
	}
	
	characters.sort();
	
	for( decltype( characters.size() ) c = 0; c < characters.size(); ++c ) {
		
		cursor.insertText( characters[ c ] + ": " );
		uint_fast8_t sceneNumber = 0;
		QString previousNameEncountered = "";
		
		for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
			if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE || block == doc->firstBlock() ) {
				
				sceneNumber += 1;
				
			}
			
			if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
				
				previousNameEncountered = block.text();
				
			} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
				
				if( characters[ c ].compare( previousNameEncountered, Qt::CaseInsensitive ) == 0 ) {
					
					cursor.insertText( QString( "Scene #" ) + std::to_string( sceneNumber ).c_str() + ": " + block.text() + ", " ); //TODO: Translate.
					
				}
				
			}
			
		}
		
		cursor.insertBlock();
		
	}
}

void ReportGenerator::parentheticalsPerCharacter( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	
	QStringList characters;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			
			if( !characters.contains( block.text(), Qt::CaseInsensitive ) ) {
				
				characters.append( block.text() );
				
			}
			
		}
		
	}
	
	characters.sort();
	
	for( decltype( characters.size() ) c = 0; c < characters.size(); ++c ) {
		
		cursor.insertText( characters[ c ] + ": " );
		uint_fast8_t sceneNumber = 0;
		QString previousNameEncountered = "";
		
		for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
			if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE || block == doc->firstBlock() ) {
				
				sceneNumber += 1;
				
			}
			
			if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
				
				previousNameEncountered = block.text();
				
			} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
				
				if( characters[ c ].compare( previousNameEncountered, Qt::CaseInsensitive ) == 0 ) {
					
					cursor.insertText( QString( "Scene #" ) + std::to_string( sceneNumber ).c_str() + ": " + block.text() + ", " ); //TODO: Translate.
					
				}
				
			}
			
		}
		
		cursor.insertBlock();
		
	}
}

void ReportGenerator::transitionsPerCharacter( QTextDocument* report ) {
	uint_fast16_t transitions = 0;
	QStringList characters;
	
	auto doc = script->textDocument();
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			transitions++;
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			if( !characters.contains( block.text(), Qt::CaseInsensitive ) ) {
				characters.append( block.text() );
			}
		}
	}
	
	QTextCursor cursor( report->firstBlock() );
	
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format );
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transitions" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Characters" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Transitions/character" ); //TODO: Translate.
	table->cellAt( 1, 0 ).firstCursorPosition().insertText( std::to_string( transitions ).c_str() );
	table->cellAt( 1, 1 ).firstCursorPosition().insertText( std::to_string( characters.length() ).c_str() );
	
	if( characters.length() > 0 ) {
		table->cellAt( 1, 2 ).firstCursorPosition().insertText( std::to_string( ( ( double ) transitions ) / characters.length() ).c_str() );
	}
	
}

void ReportGenerator::shotsPerCharacter( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	
	QStringList characters;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			
			if( !characters.contains( block.text(), Qt::CaseInsensitive ) ) {
				characters.append( block.text() );
			}
			
		}
		
	}
	
	characters.sort();
	
	for( decltype( characters.size() ) c = 0; c < characters.size(); ++c ) {
		
		cursor.insertText( characters[ c ] + ": " );
		uint_fast8_t sceneNumber = 0;
		
		for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
			if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE || block == doc->firstBlock() ) {
				sceneNumber += 1;
			}
			
			if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
				
				if( block.text().contains( characters[ c ], Qt::CaseInsensitive ) ) {
					
					cursor.insertText( QString( "Scene #" ) + std::to_string( sceneNumber ).c_str() + ": " + block.text() + ", " ); //TODO: Translate.
					
				}
				
			}
			
		}
		
		cursor.insertBlock();
		
	}
}

void ReportGenerator::actBreaksPerCharacter( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	
	QStringList characters;
	QVector< QStringList > acts;
	QString currentAct = "Act 1"; //TODO: Translate.
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			
			if( !characters.contains( block.text(), Qt::CaseInsensitive ) ) {
				characters.append( block.text() );
			}
			
			if( acts.length() <= characters.indexOf( block.text() ) ) {
				acts.resize( characters.indexOf( block.text() ) + 1 );
			}
			
			acts[ characters.indexOf( block.text() ) ].append( currentAct );
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
			
			currentAct = block.text();
			
		}
		
	}
	
	for( uint_fast8_t c = 0; c < acts.length(); ++c ) {
		acts[ c ] = acts[ c ].toSet().toList(); //Converting to a QSet and back removes duplicates
	}
	
	//TODO: Can we sort characters without messing up acts?
	
	for( decltype( characters.size() ) c = 0; c < characters.size(); ++c ) {
		
		cursor.insertText( characters[ c ] + " is in " ); //TODO: Translate.
		
		for( decltype( acts[ c ].size() ) s = 0; s < acts[ c ].size(); ++s ) {
			cursor.insertText( acts[ c ][ s ] + ", " );
		}
		
		cursor.insertBlock();
		
	}
}

void ReportGenerator::scenesPerDialog( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	bool foundAnything = false;
	QString dialog = "";
	bool firstDialogFound = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
			if( Q_LIKELY( firstDialogFound ) ) {
				cursor.insertBlock();
			} else {
				firstDialogFound = true;
			}
			cursor.insertText( block.text() + ": " + dialog );
			foundAnything = true;
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			
			foundAnything = false;
			dialog = block.text();
			
		}
	}
	
	if( !foundAnything ) {
		//cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::actionsPerDialog( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: dialog, action before, and action after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Dialog" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Action preceding" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Action following" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			if( Q_LIKELY( currentRow > 1 ) ) {
				table->cellAt( currentRow - 1, 2 ).firstCursorPosition().insertText( block.text() );
			}
			table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( block.text() );
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::parentheticalsPerDialog( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
			cursor.insertText( block.text() + ": " );
			if( block != doc->firstBlock() && block.previous().userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
				cursor.insertText( block.previous().text() );
			}
			cursor.insertBlock();
		}
	}
}

void ReportGenerator::transitionsPerDialog( QTextDocument* report ) {
	uint_fast16_t transitions = 0;
	uint_fast16_t dialogs = 0;
	
	auto doc = script->textDocument();
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			transitions++;
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
			dialogs++;
		}
	}
	
	QTextCursor cursor( report->firstBlock() );
	
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format );
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transitions" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Dialogs" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Transitions/dialog" ); //TODO: Translate.
	table->cellAt( 1, 0 ).firstCursorPosition().insertText( std::to_string( transitions ).c_str() );
	table->cellAt( 1, 1 ).firstCursorPosition().insertText( std::to_string( dialogs ).c_str() );
	
	if( dialogs > 0 ) {
		table->cellAt( 1, 2 ).firstCursorPosition().insertText( std::to_string( ( ( double ) transitions ) / dialogs ).c_str() );
	}
	
}

void ReportGenerator::scenesPerParenthetical( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	uint_fast8_t sceneNumber = 0;
	auto doc = script->textDocument();
	bool foundAnything = false;
	QString scene = "";
	bool firstActionFound = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
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

void ReportGenerator::actionsPerParenthetical( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: parenthetical, action before, and action after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Parenthetical" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Action preceding" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Action following" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
			if( Q_LIKELY( currentRow > 1 ) ) {
				table->cellAt( currentRow - 1, 2 ).firstCursorPosition().insertText( block.text() );
			}
			table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( block.text() );
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::charactersPerParenthetical( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QString character = "";
	auto doc = script->textDocument();
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
			if( block != doc->firstBlock() && character.isEmpty()) {
				cursor.insertText( "None found" ); //TODO: Translate
			}
			
			cursor.insertText( block.text() + ": " );
			
			if( !character.isEmpty() ) {
				cursor.insertText( character );
			}
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			
			character = "";
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
			
			character = block.text();
			
		}
	}
	
}

void ReportGenerator::dialogsPerParenthetical( QTextDocument* report ) {
	
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
			QTextBlock next;
			do {
				next = block.next();
			} while( next.isValid() && next.userState() != ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG );
			
			cursor.insertText( block.text() + ": " );
			cursor.insertText( next.text() );
			
			if( block != doc->firstBlock() ) {
				cursor.insertBlock();
			}
			
		}
	}
	
}

void ReportGenerator::transitionsPerParenthetical( QTextDocument* report ) {
	uint_fast16_t transitions = 0;
	uint_fast16_t parentheticals = 0;
	
	auto doc = script->textDocument();
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			transitions++;
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
			parentheticals++;
		}
	}
	
	QTextCursor cursor( report->firstBlock() );
	
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format );
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transitions" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Parentheticals" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Transitions/parenthetical" ); //TODO: Translate.
	table->cellAt( 1, 0 ).firstCursorPosition().insertText( std::to_string( transitions ).c_str() );
	table->cellAt( 1, 1 ).firstCursorPosition().insertText( std::to_string( parentheticals ).c_str() );
	
	if( parentheticals > 0 ) {
		table->cellAt( 1, 2 ).firstCursorPosition().insertText( std::to_string( ( ( double ) transitions ) / parentheticals ).c_str() );
	}
	
}

void ReportGenerator::shotsPerParenthetical( QTextDocument* report ) {
	auto doc = script->textDocument();
	QString shot = "";
	QTextCursor cursor( report->firstBlock() );
	
	for( QTextBlock block = doc->firstBlock(); block != doc->end(); block = block.next() ) {
		switch( block.userState() ) {
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
				shot = block.text();
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE:
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION:
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK: {
				shot = "This parenthetical is not in a shot"; //TODO: Translate.
				break;
			}
			case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
				cursor.insertText( block.text() + ": " + shot );
				cursor.insertBlock();
				break;
			}
			default: break;
		}
		
	}
}

void ReportGenerator::actBreaksPerParenthetical( QTextDocument* report ) {
	uint_fast16_t actBreaks = 0;
	uint_fast16_t parentheticals = 0;
	
	auto doc = script->textDocument();
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
			actBreaks++;
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
			parentheticals++;
		}
	}
	
	QTextCursor cursor( report->firstBlock() );
	
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format );
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Act breaks" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Parentheticals" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Act breaks/parenthetical" ); //TODO: Translate.
	table->cellAt( 1, 0 ).firstCursorPosition().insertText( std::to_string( actBreaks ).c_str() );
	table->cellAt( 1, 1 ).firstCursorPosition().insertText( std::to_string( parentheticals ).c_str() );
	
	if( parentheticals > 0 ) {
		table->cellAt( 1, 2 ).firstCursorPosition().insertText( std::to_string( ( ( double ) actBreaks ) / parentheticals ).c_str() );
	}
	
}

void ReportGenerator::scenesPerTransition( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: transition, scene before, and scene after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transition" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Scene preceding" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Scene following" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			
			{
				QTextBlock preceding = block;
				bool found = false;
				do {
					preceding = preceding.previous();
					if( preceding.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
						table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( preceding.text() );
						found = true;
					}
				} while( !found && preceding.length() > 0 );
			}
			{
				QTextBlock following = block;
				bool found = false;
				do {
					following = following.next();
					if( following.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
						table->cellAt( currentRow, 2 ).firstCursorPosition().insertText( following.text() );
						found = true;
					}
				} while( !found && following.length() > 0 );
			}
			
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::actionsPerTransition( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: transition, before, and after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transition" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Action preceding" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Action following" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			
			{
				QTextBlock preceding = block;
				bool found = false;
				do {
					preceding = preceding.previous();
					if( preceding.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
						table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( preceding.text() );
						found = true;
					}
				} while( !found && preceding.length() > 0 );
			}
			{
				QTextBlock following = block;
				bool found = false;
				do {
					following = following.next();
					if( following.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
						table->cellAt( currentRow, 2 ).firstCursorPosition().insertText( following.text() );
						found = true;
					}
				} while( !found && following.length() > 0 );
			}
			
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::charactersPerTransition( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: transition, before, and after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transition" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Characters in preceding scene" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Characters in following scene" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			
			{
				QTextBlock preceding = block;
				bool found = false;
				QStringList characters;
				
				do {
					preceding = preceding.previous();
					if( preceding.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
						if( !characters.contains( preceding.text(), Qt::CaseInsensitive ) ) {
							characters.append( preceding.text() );
						}
					} else if( preceding.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
						found = true;
					}
				} while( !found && preceding.length() > 0 );
				
				characters.sort();
				table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( characters.join( ", " ) );
			}
			{
				QTextBlock following = block.next();
				if( following.length() > 0 ) {
					bool found = false;
					QStringList characters;
					
					do {
						following = following.next();
						if( following.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
							if( !characters.contains( following.text(), Qt::CaseInsensitive ) ) {
								characters.append( following.text() );
							}
						} else if( following.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
							found = true;
						}
					} while( !found && following.length() > 0 );
					
					characters.sort();
					table->cellAt( currentRow, 2 ).firstCursorPosition().insertText( characters.join( ", " ) );
				}
			}
			
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::dialogsPerTransition( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: transition, before, and after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transition" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Dialog preceding" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Dialog following" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			
			{
				QTextBlock preceding = block;
				bool found = false;
				do {
					preceding = preceding.previous();
					if( preceding.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
						table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( preceding.text() );
						found = true;
					}
				} while( !found && preceding.length() > 0 );
			}
			{
				QTextBlock following = block;
				bool found = false;
				do {
					following = following.next();
					if( following.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
						table->cellAt( currentRow, 2 ).firstCursorPosition().insertText( following.text() );
						found = true;
					}
				} while( !found && following.length() > 0 );
			}
			
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::parentheticalsPerTransition( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: transition, before, and after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transition" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Parenthetical preceding" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Parenthetical following" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			
			{
				QTextBlock preceding = block;
				bool found = false;
				do {
					preceding = preceding.previous();
					if( preceding.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
						table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( preceding.text() );
						found = true;
					}
				} while( !found && preceding.length() > 0 );
			}
			{
				QTextBlock following = block;
				bool found = false;
				do {
					following = following.next();
					if( following.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
						table->cellAt( currentRow, 2 ).firstCursorPosition().insertText( following.text() );
						found = true;
					}
				} while( !found && following.length() > 0 );
			}
			
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::shotsPerTransition( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: transition, before, and after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transition" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Parenthetical preceding" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Parenthetical following" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			
			{
				QTextBlock preceding = block;
				bool found = false;
				do {
					preceding = preceding.previous();
					if( preceding.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
						table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( preceding.text() );
						found = true;
					}
				} while( !found && preceding.length() > 0 );
			}
			{
				QTextBlock following = block;
				bool found = false;
				do {
					following = following.next();
					if( following.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
						table->cellAt( currentRow, 2 ).firstCursorPosition().insertText( following.text() );
						found = true;
					}
				} while( !found && following.length() > 0 );
			}
			
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::actBreaksPerTransition( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	QTextDocument* doc = script->textDocument();
	QTextTableFormat format;
	format.setHeaderRowCount( 1 );
	QTextTable* table = cursor.insertTable( 2, 3, format ); //Start with only 2 rows; others will be added dynamically. We do know in advance there will only be 3 columns: transition, before, and after.
	table->cellAt( 0, 0 ).firstCursorPosition().insertText( "Transition" ); //TODO: Translate.
	table->cellAt( 0, 1 ).firstCursorPosition().insertText( "Act break preceding" ); //TODO: Translate.
	table->cellAt( 0, 2 ).firstCursorPosition().insertText( "Act break following" ); //TODO: Translate.
	
	uint_fast8_t currentRow = 1;
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
			table->cellAt( currentRow, 0  ).firstCursorPosition().insertText( block.text() );
			
			{
				QTextBlock preceding = block;
				bool found = false;
				do {
					preceding = preceding.previous();
					if( preceding.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
						table->cellAt( currentRow, 1 ).firstCursorPosition().insertText( preceding.text() );
						found = true;
					}
				} while( !found && preceding.length() > 0 );
			}
			{
				QTextBlock following = block;
				bool found = false;
				do {
					following = following.next();
					if( following.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
						table->cellAt( currentRow, 2 ).firstCursorPosition().insertText( following.text() );
						found = true;
					}
				} while( !found && following.length() > 0 );
			}
			
			currentRow++;
			table->appendRows( 1 );
		}
	}
}

void ReportGenerator::scenesPerShot( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	bool foundAnything = false;
	QString shot = "";
	bool firstShotFound = false;
	
	for( QTextBlock block = doc->begin(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			if( Q_LIKELY( firstShotFound ) ) {
				cursor.insertBlock();
			} else {
				firstShotFound = true;
			}
			cursor.insertText( block.text() + ": " + shot );
			foundAnything = true;
			
		} else if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
			
			foundAnything = false;
			shot = block.text();
			
		}
	}
	
	if( !foundAnything ) {
		//cursor.insertText( "None found" ); //TODO: Translate
	}
}

void ReportGenerator::actionsPerShot( QTextDocument* report ) {
	QTextDocument* doc = script->textDocument();
	QTextCursor cursor( report->firstBlock() );
	
	for( QTextBlock block = doc->firstBlock(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			cursor.insertText( block.text() + ": " );
			
			bool keepGoing = true;
			for( QTextBlock innerBlock = block.next(); innerBlock != doc->end() && keepGoing; innerBlock = innerBlock.next() ) {
				if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT 
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
					keepGoing = false;
				} else if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION ) {
					cursor.insertText( innerBlock.text() + ", " );
				}
			}
		}
	}
}

void ReportGenerator::charactersPerShot( QTextDocument* report ) {
	QTextDocument* doc = script->textDocument();
	QTextCursor cursor( report->firstBlock() );
	
	for( QTextBlock block = doc->firstBlock(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			cursor.insertText( block.text() + ": " );
			
			bool keepGoing = true;
			for( QTextBlock innerBlock = block.next(); innerBlock != doc->end() && keepGoing; innerBlock = innerBlock.next() ) {
				if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT 
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
					keepGoing = false;
				} else if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER ) {
					cursor.insertText( innerBlock.text() + ", " );
				}
			}
		}
	}
}

void ReportGenerator::dialogsPerShot( QTextDocument* report ) {
	QTextDocument* doc = script->textDocument();
	QTextCursor cursor( report->firstBlock() );
	
	for( QTextBlock block = doc->firstBlock(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			cursor.insertText( block.text() + ": " );
			
			bool keepGoing = true;
			for( QTextBlock innerBlock = block.next(); innerBlock != doc->end() && keepGoing; innerBlock = innerBlock.next() ) {
				if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT 
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
					keepGoing = false;
				} else if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG ) {
					cursor.insertText( innerBlock.text() + ", " );
				}
			}
		}
	}
}

void ReportGenerator::parentheticalsPerShot( QTextDocument* report ) {
	QTextDocument* doc = script->textDocument();
	QTextCursor cursor( report->firstBlock() );
	
	for( QTextBlock block = doc->firstBlock(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			cursor.insertText( block.text() + ": " );
			
			bool keepGoing = true;
			for( QTextBlock innerBlock = block.next(); innerBlock != doc->end() && keepGoing; innerBlock = innerBlock.next() ) {
				if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT 
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
					keepGoing = false;
				} else if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL ) {
					cursor.insertText( innerBlock.text() + ", " );
				}
			}
		}
	}
}

void ReportGenerator::transitionsPerShot( QTextDocument* report ) {
	QTextDocument* doc = script->textDocument();
	QTextCursor cursor( report->firstBlock() );
	
	for( QTextBlock block = doc->firstBlock(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			cursor.insertText( block.text() + ": " );
			
			bool keepGoing = true;
			for( QTextBlock innerBlock = block.next(); innerBlock != doc->end() && keepGoing; innerBlock = innerBlock.next() ) {
				if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
					keepGoing = false;
				} else if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION ) {
					cursor.insertText( innerBlock.text() + ", " );
				}
			}
		}
	}
}

void ReportGenerator::actBreaksPerShot( QTextDocument* report ) {
	QTextDocument* doc = script->textDocument();
	QTextCursor cursor( report->firstBlock() );
	
	for( QTextBlock block = doc->firstBlock(); block != doc->end(); block = block.next() ) {
		if( block.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT ) {
			cursor.insertText( block.text() + ": " );
			
			bool keepGoing = true;
			for( QTextBlock innerBlock = block.next(); innerBlock != doc->end() && keepGoing; innerBlock = innerBlock.next() ) {
				if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT 
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION
						|| innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE ) {
					keepGoing = false;
				} else if( innerBlock.userState() == ( uint_fast8_t ) ScriptFormatter::paragraphType::ACT_BREAK ) {
					cursor.insertText( innerBlock.text() + ", " );
				}
			}
		}
	}
}

QString ReportGenerator::generateReport( int numerator, int denominator ) {
	QTextDocument report;
	
	if( numerator == denominator ) {
		return report.toHtml();
	}
	
	QString unimplemented = "Not implemented yet"; //TODO: translate
	
	switch( numerator ) {
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
					scenesPerAction( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					scenesPerCharacter( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG: {
					scenesPerDialog( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
					scenesPerParenthetical( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
					scenesPerTransition( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
					scenesPerShot( &report );
					break;
				}
				default: {
					report.setPlainText( unimplemented );
					break;
				}
			}

			break;
		}
		
		case ( uint_fast8_t ) ScriptFormatter::paragraphType::ACTION: {
			switch( denominator ) {
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SCENE: {
					actionsPerScene( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					actionsPerCharacter( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG: {
					actionsPerDialog( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
					actionsPerParenthetical( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
					actionsPerTransition( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
					actionsPerShot( &report );
					break;
				}
				default: {
					report.setPlainText( unimplemented );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
					charactersPerParenthetical( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
					charactersPerTransition( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
					charactersPerShot( &report );
					break;
				}
				default: {
					report.setPlainText( unimplemented );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					dialogsPerCharacter( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
					dialogsPerParenthetical( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
					dialogsPerTransition( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
					dialogsPerShot( &report );
					break;
				}
				default: {
					report.setPlainText( unimplemented );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					parentheticalsPerCharacter( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG: {
					parentheticalsPerDialog( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
					parentheticalsPerTransition( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
					parentheticalsPerShot( &report );
					break;
				}
				default: {
					report.setPlainText( unimplemented );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					transitionsPerCharacter( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::DIALOG: {
					transitionsPerDialog( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
					transitionsPerParenthetical( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
					transitionsPerShot( &report );
					break;
				}
				default: {
					report.setPlainText( unimplemented );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					shotsPerCharacter( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
					shotsPerParenthetical( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
					shotsPerTransition( &report );
					break;
				}
				default: {
					report.setPlainText( unimplemented );
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
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					actBreaksPerCharacter( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::PARENTHETICAL: {
					actBreaksPerParenthetical( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::TRANSITION: {
					actBreaksPerTransition( &report );
					break;
				}
				case ( uint_fast8_t ) ScriptFormatter::paragraphType::SHOT: {
					actBreaksPerShot( &report );
					break;
				}
				default: {
					report.setPlainText( unimplemented );
					break;
				}
			}
			break;
		}
		
	}
	
	return report.toHtml();
}

Q_INVOKABLE void ReportGenerator::setScript( QQuickTextDocument* newScript ) {
	script = newScript;
}
