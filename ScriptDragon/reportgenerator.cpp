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

void ReportGenerator::scenesPerCharacter( QTextDocument* report ) {
	QTextCursor cursor( report->firstBlock() );
	auto doc = script->textDocument();
	
	QStringList characters;
	QVector< QStringList > scenes;
	QString currentScene = "Unnamed first scene";
	
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
	QTextCursor cursor( report->firstBlock() );
	cursor.insertText( "I don't know how to deal with this combination" ); //TODO: translate
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
				case( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					scenesPerCharacter( &report );
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
				case( uint_fast8_t ) ScriptFormatter::paragraphType::CHARACTER: {
					dialogsPerCharacter( &report );
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
