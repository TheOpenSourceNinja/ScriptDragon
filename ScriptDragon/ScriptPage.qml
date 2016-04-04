import QtQuick 2.0
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0
import QtQuick.Dialogs 1.2
import Ubuntu.Components.Pickers 1.3
import QtQuick.Controls 1.4 as Controls

Page {
	property alias text: scriptTA.text
	
	Column {
		width: parent.width
		height: parent.height
		
		id: mainColumn
		
		Row {
			id: buttonRow
			
			FileDialog {
				id: exportDialog
				title: i18n.tr( "Choose a file to export to" )
				folder: shortcuts.documents
				
				selectExisting: false //From the docs: "Whether only existing files or directories can be selected... Setting this property to false implies that the dialog is for naming a file to which to save something... therefore selectMultiple must be false."
				selectMultiple: false //If this were true then the user couldn't input a file name
				
				nameFilters: [
					i18n.tr( "Infer format from file extension" ) + " (*.html *.htm *.odt *.pdf *.png *.txt *)",
					i18n.tr( "HyperText Markup Language" ) + " (*.html *.htm)",
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
									
									if( format == "htm" ) {
										format = "html"
									}
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
								var nameParts = fileUrl.toString().split( "." )
								
								var extensionFound = false
								var possibleExtension = nameParts.pop().toLowerCase()
								for( var i = 0; i < selectedNameFilterExtensions.length; ++i ) {
									if( possibleExtension == selectedNameFilterExtensions[ i ].split( "." ).pop().toLowerCase() ) {
										extensionFound = true
										break
									}
								}
								
								if( extensionFound ) {
									urlToUse = fileUrl.toString()
								} else {
									urlToUse = fileUrl.toString() + "." + format
								}
								
								urlToUse = url(urlToUse);
								
								console.log(urlToUse)
								break;
							}
							default: {
								format = "txt";
								break;
							}
						}
					}
					
					console.log( format );
					
					if( format == "html" ) {
						ExportManager.textDocumentToHTMLFile( scriptTA.textDocument, urlToUse );
					}  else if( format == "odt" ) {
						ExportManager.textDocumentToOpenDocumentFile( scriptTA.textDocument, urlToUse );
					} else if( format == "pdf" ) {
						ExportManager.textDocumentToPDF( scriptTA.textDocument, urlToUse );
					} else if( format == "png" ) {
						scriptTA.forceActiveFocus() //ensures the background is properly whitened
						scriptTA.grabToImage( function( result ) { //FIXME: This just takes a screenshot, does not scroll
							var s = urlToUse.toString().substring(7); //Remove the file:// from the beginning
							if( result.saveToFile( s ) ) {
								console.log( "Saved image to file " + s );
							} else {
								console.log( "Could not save image to file " + s );
							}
						})
					} else { //Plain text
						ExportManager.textDocumentToPlainTextFile( scriptTA.textDocument, urlToUse );
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
					ExportManager.textDocumentToPrintout( scriptTA.textDocument )
				}
			}
		}
		
		Picker {
			id: typePicker
			model: ["Scene", "Action", "Character", "Dialog", "Parenthetical", "Transition", "Shot", "Act break" ]; //This must EXACTLY MATCH the paragraphType enum in scriptformatter.h!
			width: units.gu(15)
			//anchors.left: parent.left
			//anchors.top: parent.top
			delegate: PickerDelegate {
				Label {
					text: modelData
					anchors.fill: parent
					verticalAlignment: Text.AlignVCenter
					horizontalAlignment: Text.AlignHCenter
				}
			}
			selectedIndex: 0
			
			onSelectedIndexChanged: {
				console.log( "selected index: " + selectedIndex );
				//scriptTA.selectAll();
				console.log( "selected text: " + scriptTA.selectedText );
				
				ScriptFormatter.setParagraphType( scriptTA.textDocument, selectedIndex, scriptTA.selectionStart, scriptTA.selectionEnd );
			}
		}
		
		Controls.TextArea {
			width: parent.width
			height: parent.height - buttonRow.height - typePicker.height
			//contentWidth: width
			//autoSize: false
			//maximumLineCount: 0
			id: scriptTA
			//mouseSelectionMode: TextEdit.SelectWords
			//color: "black" //This is the color of the text, not of the text area itself
			textFormat: TextEdit.RichText
			//persistentSelection: true //Documentation says this is true by default. Experimentation says otherwise.
			
			/*Component.onCompleted: {
				font.setStyleHint( Qt.Typewriter )
			}*/
			
			Component.onCompleted: {
				ScriptFormatter.setDefaultFontForDocument( textDocument );
			}
			
			//font: ScriptFormatter.getDefaultFont();
			/*font: Qt.font( { //This is a fontSpecifier object; see https://developer.ubuntu.com/api/apps/qml/sdk-15.04.1/QtQml.Qt/
							  "family":"Courier",
							  "styleHint":Qt.Typewriter,
							  "pointSize":12,
							  "fixedPitch":true //Can't tell if this actually does anything since invalid properties are ignored by fontSpecifier objects
							  //"bold":true,
							  //"italic":true
						  } )*/
	
			//placeholderText: i18n.tr( "Type here" )
		}
	}
}
