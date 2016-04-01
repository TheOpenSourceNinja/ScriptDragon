#ifndef PRINTMANAGER_H
#define PRINTMANAGER_H

#include <QObject>
#include <QtPrintSupport/QPrinter>
//#include <QPdfWriter>
#include <QQmlEngine>
#include <QQuickTextDocument>

class PrintManager : public QObject {
		Q_OBJECT
	public:
		PrintManager( QObject *parent = 0 ) : QObject( parent ) {} //, PDFWriter( "test.pdf" ) {}
		
		Q_INVOKABLE void textDocumentToPrintout( QQuickTextDocument* document );
		Q_INVOKABLE void textDocumentToPDF( QQuickTextDocument* document, const QUrl& fileURL );
	signals:
		
	public slots:
		
	private:
		//QPdfWriter PDFWriter;
		QPrinter printer;
		
		void common( const QString& docName );
		
		bool showPrintDialog( const QString& windowTitle );
};

//Define the singleton type provider function
static QObject* printManager_provider( QQmlEngine* engine, QJSEngine* scriptEngine ) {
	Q_UNUSED( engine )
	Q_UNUSED( scriptEngine )
	
	PrintManager* pm = new PrintManager();
	return pm;
}

#endif // PRINTMANAGER_H
