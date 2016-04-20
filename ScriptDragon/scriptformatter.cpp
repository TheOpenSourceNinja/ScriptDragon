#include <iostream>
#include "scriptformatter.h"
#include <QRegularExpression>
#include <QTextBlock>

ScriptFormatter::ScriptFormatter( QQmlEngine* newEngine, QObject *parent ) : QObject(parent) {
	QFont baseFont; //The base from which all other fonts are derived
	baseFont.setBold( false );
	baseFont.setCapitalization( QFont::MixedCase );
	baseFont.setFamily( "Courier" );
	baseFont.setItalic( false );
	baseFont.setPointSize( 12 );
	baseFont.setStyleHint( QFont::Courier );
	
	QTextBlockFormat baseFormat;
	baseFormat.setAlignment( Qt::AlignLeft );
	baseFormat.setIndent( 0 );
	baseFormat.setLineHeight( 1, QTextBlockFormat::SingleHeight ); //The first argument should be ignored according to the documentation, since we're setting the LineHeightType (2nd argument) to single height
	baseFormat.setNonBreakableLines( false );
	//baseFormat.setPageBreakPolicy( QTextFormat::PageBreak_Auto );
	baseFormat.setTextIndent( 0 );
	baseFormat.setTopMargin( 0 );
	baseFormat.setBottomMargin( 0 );
	baseFormat.setLeftMargin( 0 );
	baseFormat.setRightMargin( 0 );
	baseFormat.setNonBreakableLines( false );
	
	//Scenes are left-aligned, bold, and all caps
	sceneFont = QFont( baseFont );
	sceneFont.setBold( true );
	sceneFont.setCapitalization( QFont::AllUppercase );
	sceneBlockFormat = QTextBlockFormat( baseFormat );
	sceneBlockFormat.setAlignment( Qt::AlignLeft );
	//sceneBlockFormat.setPageBreakPolicy( QTextFormat::PageBreak_AlwaysBefore );
	
	//Actions are left-aligned
	actionFont = QFont( baseFont );
	actionBlockFormat = QTextBlockFormat( baseFormat );
	actionBlockFormat.setAlignment( Qt::AlignLeft );
	
	//Character names are centered and all caps
	characterFont = QFont( baseFont );
	characterFont.setCapitalization( QFont::AllUppercase );
	characterBlockFormat = QTextBlockFormat( baseFormat );
	characterBlockFormat.setAlignment( Qt::AlignHCenter );
	//characterBlockFormat.setPageBreakPolicy( QTextFormat::PageBreak_AlwaysBefore );
	
	//Dialog is centered
	dialogFont = QFont( baseFont );
	dialogBlockFormat = QTextBlockFormat( baseFormat );
	dialogBlockFormat.setAlignment( Qt::AlignHCenter );
	//dialogBlockFormat.setPageBreakPolicy( QTextFormat::PageBreak_AlwaysAfter );
	
	//Parentheticals are centered and italicized
	parentheticalFont = QFont( baseFont );
	parentheticalFont.setItalic( true );
	parentheticalBlockFormat = QTextBlockFormat( baseFormat );
	parentheticalBlockFormat.setAlignment( Qt::AlignHCenter );
	
	//Transitions are right-aligned and all caps
	transitionFont = QFont( baseFont );
	transitionFont.setCapitalization( QFont::AllUppercase );
	transitionBlockFormat = QTextBlockFormat( baseFormat );
	transitionBlockFormat.setAlignment( Qt::AlignRight );
	
	//Shots are left-aligned and all caps
	shotFont = QFont( baseFont );
	shotFont.setCapitalization( QFont::AllUppercase );
	shotBlockFormat = QTextBlockFormat( baseFormat );
	shotBlockFormat.setAlignment( Qt::AlignLeft );
	
	//Act breaks are centered, bold, and underlined
	actBreakFont = QFont( baseFont );
	actBreakFont.setBold( true );
	actBreakFont.setUnderline( true );
	actBreakBlockFormat = QTextBlockFormat( baseFormat );
	actBreakBlockFormat.setAlignment( Qt::AlignHCenter );
	
}

int ScriptFormatter::getFormat( QQuickTextDocument* document, int cursorPosition ) {
	return document->textDocument()->findBlock( cursorPosition ).userState();
}

void ScriptFormatter::setDefaultFontForDocument( QQuickTextDocument* document ) {
	document->textDocument()->setDefaultFont( sceneFont );
	
	/*QTextCursor cursor( document->textDocument()->firstBlock() );
	cursor.select( QTextCursor::LineUnderCursor );
	cursor.block().setUserState( SCENE );
	cursor.setBlockFormat( sceneBlockFormat );
	QTextCharFormat charFormat( cursor.charFormat() );
	charFormat.setFont( sceneFont );
	cursor.setCharFormat( charFormat );
	cursor.setBlockCharFormat( charFormat );*/
	setParagraphType( document, paragraphType::SCENE, 0 );
}

void ScriptFormatter::setParagraphType( QQuickTextDocument* document, ScriptFormatter::paragraphType newType, int cursorPosition ) {
	std::cout << "setParagraphType() called. newType: " << (uint_fast8_t) newType << std::endl;
	//std::cout << document->textDocument()->toHtml().toStdString().c_str() << std::endl;
	//std::cout << cursorPosition << std::endl;
	std::cout << "Block text: " << document->textDocument()->findBlock(cursorPosition).text().toStdString().c_str() << std::endl;
	
	QTextCursor cursor( document->textDocument()->findBlock( cursorPosition ) );
	cursor.select( QTextCursor::BlockUnderCursor );
	
	std::cout << "Selected text: " << cursor.selectedText().toStdString().c_str() << std::endl;
	
	//cursor.setPosition( cursorPosition, QTextCursor::MoveAnchor);
	cursor.beginEditBlock();
	QTextCharFormat charFormat( cursor.charFormat() );
	QTextBlockFormat blockFormat;
	
	switch( newType ) {
		case paragraphType::SCENE: {
			charFormat.setFont( sceneFont );
			blockFormat = sceneBlockFormat;
			break;
		}
		case paragraphType::ACTION: {
			charFormat.setFont( actionFont );
			blockFormat = actionBlockFormat;
			break;
		}
		case paragraphType::CHARACTER: {
			charFormat.setFont( characterFont );
			blockFormat = characterBlockFormat;
			break;
		}
		case paragraphType::DIALOG: {
			charFormat.setFont( dialogFont );
			blockFormat = dialogBlockFormat;
			break;
		}
		case paragraphType::PARENTHETICAL: {
			charFormat.setFont( parentheticalFont );
			blockFormat = parentheticalBlockFormat;
			
			if( !cursor.selectedText().startsWith( '(' ) ) {
				auto position = cursor.position();
				
				cursor.movePosition( QTextCursor::StartOfBlock );
				cursor.insertText( "(" );
				
				cursor.setPosition( position );
				cursor.select( QTextCursor::BlockUnderCursor );
			}
			
			if( !cursor.selectedText().endsWith( ')' ) ) {
				auto position = cursor.position();
				
				cursor.movePosition( QTextCursor::EndOfBlock );
				cursor.insertText( ")" );
				
				cursor.setPosition( position );
				cursor.select( QTextCursor::BlockUnderCursor );
			}
			
			break;
		}
		case paragraphType::TRANSITION: {
			charFormat.setFont( transitionFont );
			blockFormat = transitionBlockFormat;
			break;
		}
		case paragraphType::SHOT: {
			charFormat.setFont( shotFont );
			blockFormat = shotBlockFormat;
			break;
		}
		case paragraphType::ACT_BREAK: {
			charFormat.setFont( actBreakFont );
			blockFormat = actBreakBlockFormat;
			break;
		}
	}
	
	//cursor.select( QTextCursor::BlockUnderCursor );
	cursor.block().setUserState( (uint_fast8_t) newType );
	cursor.setCharFormat( charFormat );
	cursor.setBlockCharFormat( charFormat );
	cursor.setBlockFormat( blockFormat );
	cursor.endEditBlock();
	//document->textDocument()->setModified( false );
	std::cout << document->textDocument()->toHtml().toStdString().c_str() << std::endl;
}

void ScriptFormatter::textChanged(QQuickTextDocument* document, unsigned int cursorPosition) {
	if( document->textDocument()->isModified() ) {
		std::cout << "document was modified by the user" << std::endl;
		//document->textDocument()->setModified( false );
		
		QTextCursor cursor( document->textDocument()->findBlock( cursorPosition ) );
		cursor.select( QTextCursor::BlockUnderCursor );
		
		if( cursor.selectedText().isEmpty() ) {
			std::cout << "selection is empty." << std::endl;
			auto previousBlock = cursor.block().previous();
			if( Q_LIKELY( previousBlock.length() > 0 ) ) { //Hopefully an empty text block will have length 0
				setParagraphType( document, nextType[ ( paragraphType ) previousBlock.userState() ], cursorPosition );
			}
		} else {
			std::cout << cursor.selectedText().toStdString().c_str() << std::endl;
		}
	} else {
		std::cout << "document was not modified by the user" << std::endl;
	}
}

