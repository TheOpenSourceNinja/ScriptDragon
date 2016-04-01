#ifndef PRINTMANAGER_H
#define PRINTMANAGER_H

#include <QObject>
#include <QtPrintSupport/QPrinter>
//#include <QPdfWriter>
#include <QQmlEngine>
#include <QQuickTextDocument>
#include <QTextDocumentWriter>

class ExportManager : public QObject {
		Q_OBJECT
	public:
		ExportManager( QObject *parent = 0 ) : QObject( parent ) {} //, PDFWriter( "test.pdf" ) {}
		
		Q_INVOKABLE void textDocumentToHTMLFile( QQuickTextDocument* document, const QUrl& fileURL );
		Q_INVOKABLE void textDocumentToOpenDocumentFile( QQuickTextDocument* document, const QUrl& fileURL );
		Q_INVOKABLE void textDocumentToPlainTextFile( QQuickTextDocument* document, const QUrl& fileURL );
		Q_INVOKABLE void textDocumentToPrintout( QQuickTextDocument* document );
		Q_INVOKABLE void textDocumentToPDF( QQuickTextDocument* document, const QUrl& fileURL );
	signals:
		
	public slots:
		
	private:
		QPrinter printer;
		QTextDocumentWriter writer;
		
		bool fileExportCommon( QQuickTextDocument* document, const QUrl& fileURL );
		void printCommon( const QString& docName );
		
		bool showPrintDialog( const QString& windowTitle );
};

//Define the singleton type provider function
static QObject* exportManager_provider( QQmlEngine* engine, QJSEngine* scriptEngine ) {
	Q_UNUSED( engine )
	Q_UNUSED( scriptEngine )
	
	ExportManager* pm = new ExportManager();
	return pm;
}

#endif // PRINTMANAGER_H
