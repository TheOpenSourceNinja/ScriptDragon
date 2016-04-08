#include "reportgenerator.h"

ReportGenerator::ReportGenerator( QQmlEngine* newEngine, QObject *parent ) : QObject(parent) {
	
}

QTextDocument* ReportGenerator::generateReport( int numerator, int denominator ) {
	QTextDocument report;
	//auto script = scriptPage->property( "text" )->textDocument();
	
	switch( denominator ) {
		case ScriptFormatter::SCENE: {
			
			
			
			
			break;
		}
	}
}

Q_INVOKABLE void ReportGenerator::setScriptPage( QObject* newPage ) {
	scriptPage = newPage;
}
