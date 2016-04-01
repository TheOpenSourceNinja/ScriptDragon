import QtQuick 2.0
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0
import QtQuick.Dialogs 1.2

Page {
	property alias text: scriptTA.text
	Column {
		width: parent.width
		height: parent.height
		
		Row {
			id: buttonRow
			
			FileDialog {
				id: exportDialog
				title: i18n.tr( "Choose a file to export to" )
				folder: shortcuts.documents
				
				selectExisting: false //From the docs: "Whether only existing files or directories can be selected... Setting this property to false implies that the dialog is for naming a file to which to save something... therefore selectMultiple must be false."
				selectMultiple: false //If this were true then the user couldn't input a file name
				
				nameFilters: [
					i18n.tr( "Infer format from file extension" ) + " (*.htm *.html *.odt *.pdf *.png *.txt *)",
					i18n.tr( "HyperText Markup Language" ) + " (*.htm *.html)",
					i18n.tr( "Open Document Format" ) + " (*.odt)",
					i18n.tr( "Portable Document Format" ) + " (*.pdf)",
					i18n.tr( "Portable Network Graphics" ) + " (*.png)",
					i18n.tr( "Plain text" ) + " (*.txt *)"
				]
		
				onAccepted: {
					var urlToUse = fileUrl
					{
						var format = "";
						
						switch( selectedNameFilterIndex ) {
							case 0: {
								var nameParts = fileUrl.toString().split( "." )
								
								if( nameParts.length > 1 ) {
									format = nameParts.pop().toLowerCase();
								} else {
									format = "txt"
								}

								break;
							}
							case 1:
							case 2:
							case 3:
							case 4: {
								format = selectedNameFilterExtensions[ 0 ].split( "." ).pop().toLowerCase();
								urlToUse = fileUrl.toString() + "." + format
								console.log(fileUrl)
								break;
							}
							default: {
								format = "txt";
								break;
							}
						}
					}
					
					console.log( format );
					
					if( format == "htm" || format == "html" ) {
						console.error( "That export format is not implemented yet" )
					}  else if( format == "odt" ) {
						console.error( "That export format is not implemented yet" )
					} else if( format == "pdf" ) {
						PrintManager.textDocumentToPDF( scriptTA.textDocument, urlToUse );
					} else if( format == "png" ) {
						scriptTA.forceActiveFocus() //ensures the background is properly whitened
						scriptTA.grabToImage( function( result ) { //FIXME: This just takes a screenshot, does not scroll
							result.saveToFile( urlToUse )
						})
					} else { //Plain text
						console.error( "That export format is not implemented yet" )
					}
					
				}
		
				onRejected: {
					//label.text = i18n.tr( "Export dialog cancelled" )
				}
			}
			
			Button {
				text: i18n.tr( "Export" )
				id: exportButton
				onClicked: {
					exportDialog.visible = true;
				}
			}
			
			Button {
				text: i18n.tr( "Print" )
				id: printButton
				onClicked: {
					PrintManager.textDocumentToPrintout( scriptTA.textDocument )
				}
			}
			
			/*//TODO: Convert all these buttons to a drop-down, pop-up, or similar.
			Label {
				text: i18n.tr( "Export: " )
			}

			Button {
				text: i18n.tr( "Image" )
				id: imageButton
				onClicked: {
					scriptTA.forceActiveFocus() //ensures the background is properly whitened
					scriptTA.grabToImage( function( result ) { //FIXME: This just takes a screenshot, does not scroll
						result.saveToFile( "script.png" )
					})
				}
			}
			
			Button {
				text: i18n.tr( "PDF" )
				id: pdfButton
				onClicked: {
					PrintManager.textDocumentToPDF( scriptTA.textDocument )
				}
			}

			Button {
				text: i18n.tr( "Print" )
				id: printButton
				onClicked: {
					PrintManager.textDocumentToPrintout( scriptTA.textDocument )
				}
			}
			
			Button {
				text: i18n.tr( "Plain text" )
				id: textButton
			}
			
			Button {
				text: i18n.tr( "ODF" )
			}
			
			Button {
				text: i18n.tr( "HTML" )
			}*/
		}
		
		TextArea {
			width: parent.width
			height: parent.height - buttonRow.height
			//contentWidth: width
			autoSize: false
			maximumLineCount: 0
			id: scriptTA
	
			color: "black" //This is the color of the text, not of the text area itself
			textFormat: TextEdit.RichText
			
			/*Component.onCompleted: {
				font.setStyleHint( Qt.Typewriter )
			}*/
			font: Qt.font( { //This is a fontSpecifier object; see https://developer.ubuntu.com/api/apps/qml/sdk-15.04.1/QtQml.Qt/
							  "family":"Courier",
							  //"styleHint":Qt.Typewriter, //TODO: Find a way to use alternate font families if Courier is unavailable. Possibly use Qt.fontFamilies() https://developer.ubuntu.com/api/apps/qml/sdk-15.04.1/QtQml.Qt/
							  "pointSize":12,
							  "fixedPitch":true //Can't tell if this actually does anything since invalid properties are ignored by fontSpecifier objects
						  } )
	
			placeholderText: i18n.tr( "Type here" )
		}
	}
}
