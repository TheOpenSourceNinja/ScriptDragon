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
	baseFormat.setPageBreakPolicy( QTextFormat::PageBreak_Auto );
	baseFormat.setTextIndent( 0 );
	baseFormat.setTopMargin( 0 );
	baseFormat.setBottomMargin( 0 );
	baseFormat.setLeftMargin( 0 );
	baseFormat.setRightMargin( 0 );
	
	//Scenes are left-aligned, bold, and all caps
	sceneFont = QFont( baseFont );
	sceneFont.setBold( true );
	sceneFont.setCapitalization( QFont::AllUppercase );
	sceneBlockFormat = QTextBlockFormat( baseFormat );
	sceneBlockFormat.setAlignment( Qt::AlignLeft );
	
	//Actions are left-aligned
	actionFont = QFont( baseFont );
	actionBlockFormat = QTextBlockFormat( baseFormat );
	actionBlockFormat.setAlignment( Qt::AlignLeft );
	
	//Character names are centered and all caps
	characterFont = QFont( baseFont );
	characterFont.setCapitalization( QFont::AllUppercase );
	characterBlockFormat = QTextBlockFormat( baseFormat );
	characterBlockFormat.setAlignment( Qt::AlignHCenter );
	
	//Dialog is centered
	dialogFont = QFont( baseFont );
	dialogBlockFormat = QTextBlockFormat( baseFormat );
	dialogBlockFormat.setAlignment( Qt::AlignHCenter );
	
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

void ScriptFormatter::setDefaultFontForDocument(QQuickTextDocument* document) {
	document->textDocument()->setDefaultFont(actionFont);
}

void ScriptFormatter::setParagraphType( QQuickTextDocument* document, ScriptFormatter::paragraphType newType, int selectionStart, int selectionEnd ) {
	std::cout << "setParagraphType() called. newType: " << newType << std::endl;
	std::cout << document->textDocument()->toHtml().toStdString().c_str() << std::endl;
	std::cout << selectionStart << "\t" << selectionEnd << std::endl;
	std::cout << document->textDocument()->findBlock(selectionStart).text().toStdString().c_str() << std::endl;
	
	QTextCursor cursor( document->textDocument()->findBlock( selectionStart ) );
	QTextCharFormat charFormat( cursor.charFormat() );
	QTextBlockFormat blockFormat;
	
	switch( newType ) {
		case SCENE: {
			charFormat.setFont( sceneFont );
			blockFormat = sceneBlockFormat;
			break;
		}
		case ACTION: {
			charFormat.setFont( actionFont );
			blockFormat = actionBlockFormat;
			break;
		}
		case CHARACTER: {
			charFormat.setFont( characterFont );
			blockFormat = characterBlockFormat;
			break;
		}
		case DIALOG: {
			charFormat.setFont( dialogFont );
			blockFormat = dialogBlockFormat;
			break;
		}
		case PARENTHETICAL: {
			charFormat.setFont( parentheticalFont );
			blockFormat = parentheticalBlockFormat;
			break;
		}
		case TRANSITION: {
			charFormat.setFont( transitionFont );
			blockFormat = transitionBlockFormat;
			break;
		}
		case ACT_BREAK: {
			charFormat.setFont( actBreakFont );
			blockFormat = actBreakBlockFormat;
			break;
		}
	}
	
	cursor.select( QTextCursor::BlockUnderCursor );
	//cursor.block().setUserData( typeTracker( newType ) );
	cursor.setCharFormat( charFormat );
	cursor.setBlockFormat( blockFormat );
	
	std::cout << document->textDocument()->toHtml().toStdString().c_str() << std::endl;
}

