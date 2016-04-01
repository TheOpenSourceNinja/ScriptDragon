#include "exportmanager.h"
#include <QPagedPaintDevice>
#include <QtPrintSupport/QPrintDialog>
#include <iostream>

bool ExportManager::fileExportCommon( QQuickTextDocument* document, const QUrl& fileURL ) {
	if( !fileURL.isValid() ) {
		std::cerr << "Invalid file URL: \"" << fileURL.toString().toStdString().c_str() << "\"" << std::endl;
		return false;
	}
	
	writer.setFileName( fileURL.toLocalFile() );
	if( writer.write( document->textDocument() ) ) {
		std::cout << "file written successfully" << std::endl;
		return true;
	} else {
		std::cerr << "file written unsuccessfully" << std::endl;
		return false;
	}
}

void ExportManager::printCommon( const QString& docName ) {
	printer.setCreator( "ScriptDragon" );
	printer.setDocName( docName );
}

Q_INVOKABLE void ExportManager::textDocumentToHTMLFile( QQuickTextDocument* document, const QUrl& fileURL ) {
	writer.setFormat( "HTML" );
	
	fileExportCommon( document, fileURL );
}

Q_INVOKABLE void ExportManager::textDocumentToOpenDocumentFile( QQuickTextDocument* document, const QUrl& fileURL ) {
	writer.setFormat( "ODF" );
	
	fileExportCommon( document, fileURL );
}

Q_INVOKABLE void ExportManager::textDocumentToPlainTextFile( QQuickTextDocument* document, const QUrl& fileURL ) {
	writer.setFormat( "plaintext" );
	
	fileExportCommon( document, fileURL );
}

Q_INVOKABLE void ExportManager::textDocumentToPrintout( QQuickTextDocument* document ) {
	printer.setOutputFileName( "" ); //Disable printing to file because printing to file causes printerName() to be blank.
	
	printCommon( "Script" );
	
	//bool shouldPrint = showPrintDialog( "Print script" ); //FIXME: Uncomment this line to cause an error.
	bool shouldPrint = true;
	
	if( shouldPrint ) {
		document->textDocument()->print( &printer );
	}
}

Q_INVOKABLE void ExportManager::textDocumentToPDF( QQuickTextDocument* document, const QUrl& fileURL ) {
	if( !fileURL.isValid() ) {
		std::cerr << "Invalid file URL: \"" << fileURL.toString().toStdString().c_str() << "\"" << std::endl;
		return;
	}
	
	//-----begin page size workaround part 1-----
	printer.setOutputFileName( "" ); //Disable printing to file so we can get the default printer's default page size.
	QPagedPaintDevice::PageSize pageSize = printer.pageSize();
	//-----end page size workaround part 1-----
	printer.setOutputFileName( fileURL.toLocalFile() ); //Enable printing to file.
	//-----begin page size workaround part 2-----
	printer.setPageSize( pageSize );
	//-----end page size workaround part 2-----
	
	printer.setOutputFormat( QPrinter::PdfFormat ); //This isn't currently necessary, since currently all file names passed to this function will have the pdf extension, but it doesn't hurt and could in the future prevent non-PDF output
	
	printCommon( fileURL.fileName() );
	
	std::cout << "textDocumentToPDF():\tfileURL: \"" << fileURL.toString().toStdString().c_str() << "\"\toutputFileName: \"" << printer.outputFileName().toStdString().c_str() << "\"" << std::endl;
	
	document->textDocument()->print( &printer );
}

bool ExportManager::showPrintDialog( const QString& windowTitle ) {
	QPrintDialog dialog(&printer);
	
	dialog.setWindowTitle( windowTitle );
	
	if( dialog.exec() == QDialog::Accepted ) {
		return true;
	} else {
		return false;
	}
}
