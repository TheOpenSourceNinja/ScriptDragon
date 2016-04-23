#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QObject>
#include <QQuickTextDocument>
#include <QString>
#include <QTextDocument>
#include "scriptformatter.h"

class ReportGenerator : public QObject
{
		Q_OBJECT
	public:
		explicit ReportGenerator( QQmlEngine* newEngine = NULL, QObject *parent = 0 );
		
		Q_INVOKABLE QString generateReport( int numerator, int denominator ); //Both parameters should be of type ScriptFormatter::paragraphType. Had to make them integers for use with QML.
		
		Q_INVOKABLE void setScript( QQuickTextDocument* newScript );
		
		
		void actionsPerScene( QTextDocument* report );
		void charactersPerScene( QTextDocument* report );
		void dialogsPerScene( QTextDocument* report );
		void parentheticalsPerScene( QTextDocument* report );
		void transitionsPerScene( QTextDocument* report );
		void shotsPerScene( QTextDocument* report );
		void actBreaksPerScene( QTextDocument* report );
		
	signals:
		
	public slots:
		
	private:
		QQuickTextDocument* script;
};

//Define the singleton type provider function
static QObject* reportGenerator_provider( QQmlEngine* newEngine, QJSEngine* scriptEngine ) {
	Q_UNUSED( scriptEngine )
	
	ReportGenerator* rg = new ReportGenerator( newEngine );
	return rg;
}

#endif // REPORTGENERATOR_H