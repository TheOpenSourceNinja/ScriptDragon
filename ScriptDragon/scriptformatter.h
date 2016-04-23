#ifndef SCRIPTFORMATTER_H
#define SCRIPTFORMATTER_H

#include <cstdint>
#include <QObject>
#include <QQmlEngine>
#include <QQuickTextDocument>
#include <QTextBlockFormat>
#include <QTextBlockUserData>
#include <unordered_map>

class ScriptFormatter : public QObject
{
		Q_OBJECT
	public:
		explicit ScriptFormatter( QQmlEngine* newEngine = NULL, QObject *parent = 0 );
		
		//NOTE: When changing the paragraphType enum, be sure to make corresponding changes in ScriptPage.qml and ReportsPage.qml
		enum class paragraphType: uint_least8_t {
			SCENE,
			ACTION,
			CHARACTER,
			DIALOG,
			PARENTHETICAL,
			TRANSITION,
			SHOT,
			ACT_BREAK
		};
		Q_ENUMS( paragraphType );
		
		std::map<paragraphType, paragraphType> nextType = {
			{ paragraphType::SCENE, paragraphType::ACTION },
			{ paragraphType::ACTION, paragraphType::ACTION },
			{ paragraphType::CHARACTER, paragraphType::DIALOG },
			{ paragraphType::DIALOG, paragraphType::CHARACTER },
			{ paragraphType::PARENTHETICAL, paragraphType::DIALOG },
			{ paragraphType::TRANSITION, paragraphType::SCENE },
			{ paragraphType::SHOT, paragraphType::ACTION },
			{ paragraphType::ACT_BREAK, paragraphType::SCENE }
		};
		
		Q_INVOKABLE void enforceFormatting( QQuickTextDocument* document );
		
		Q_INVOKABLE int getFormat( QQuickTextDocument* document, int cursorPosition );
		
		Q_INVOKABLE void setDefaultFontForDocument( QQuickTextDocument* document );
		
		Q_INVOKABLE void setParagraphType( QQuickTextDocument* document, paragraphType newType, int cursorPosition );
		
		Q_INVOKABLE void textChanged( QQuickTextDocument* document, unsigned int cursorPosition );
		
		QFont sceneFont;
		QTextBlockFormat sceneBlockFormat;
		QFont actionFont;
		QTextBlockFormat actionBlockFormat;
		QFont characterFont;
		QTextBlockFormat characterBlockFormat;
		QFont dialogFont;
		QTextBlockFormat dialogBlockFormat;
		QFont parentheticalFont;
		QTextBlockFormat parentheticalBlockFormat;
		QFont transitionFont;
		QTextBlockFormat transitionBlockFormat;
		QFont shotFont;
		QTextBlockFormat shotBlockFormat;
		QFont actBreakFont;
		QTextBlockFormat actBreakBlockFormat;
		
	signals:
		
	public slots:
		
	private:
		
};

//Define the singleton type provider function
static QObject* scriptFormatter_provider( QQmlEngine* newEngine, QJSEngine* scriptEngine ) {
	Q_UNUSED( scriptEngine )
	
	ScriptFormatter* sf = new ScriptFormatter( newEngine );
	return sf;
}

#endif // SCRIPTFORMATTER_H
