import QtQuick 2.0
import ninja.theopensource.scriptdragon 1.0
import QtQuick.Dialogs 1.2
import Ubuntu.Components.Pickers 1.3
import QtQuick.Controls 1.4 as Controls
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Page {
	property alias text: scriptTA.text
	
	Connections {
		target: FileSaverAndLoader
		onFileLoaded: {
			ScriptFormatter.enforceFormatting( scriptTA.textDocument );
		}
	}
	
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
			
			ActionBar {
				actions: [
					Action {
						iconName: "save-as"
						text: i18n.tr( "Export" )
						description: i18n.tr( "Export the script as a PDF, ODT, HTML, ...")
						keywords: i18n.tr( "export;PDF;ODT;HTML;TXT;plain text" );
						onTriggered: {
							exportDialog.visible = true;
						}
					},
					Action {
						iconName: "document-print"
						text: i18n.tr( "Print" )
						description: i18n.tr( "Print the script" );
						keywords: i18n.tr( "print;default printer" )
						onTriggered: {
							ExportManager.textDocumentToPrintout( scriptTA.textDocument )
						}
					}
				]
			}
		}
		
		Button {
			id: typeButton
			text: i18n.tr( "Change paragraph type" )
			
			Component {
				id: formatDialogComponent
				Dialog {
					id: formatDialog
					
					property var index
					
					Picker {
						id: typePicker
						objectName: "typePicker"
						model: ListModel {
							Component.onCompleted: {
								insert( ScriptFormatter.SCENE, { "name": i18n.tr( "scene" ) } )
								insert( ScriptFormatter.ACTION, { "name": i18n.tr( "action" ) } )
								insert( ScriptFormatter.CHARACTER, { "name": i18n.tr( "character" ) } )
								insert( ScriptFormatter.DIALOG, { "name": i18n.tr( "dialog" ) } )
								insert( ScriptFormatter.PARENTHETICAL, { "name": i18n.tr( "parenthetical" ) } )
								insert( ScriptFormatter.TRANSITION, { "name": i18n.tr( "transition" ) } )
								insert( ScriptFormatter.SHOT, { "name": i18n.tr( "shot" ) } )
								insert( ScriptFormatter.ACT_BREAK, { "name": i18n.tr( "act break" ) } )
							}
						}
						
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
						
						selectedIndex: formatDialog.index
					}
					
					Button {
						text: i18n.tr( "OK" );
						onClicked: {
							ScriptFormatter.setParagraphType( scriptTA.textDocument, typePicker.selectedIndex, scriptTA.cursorPosition );
							PopupUtils.close( formatDialog );
						}
					}
					
					Button {
						text: i18n.tr( "Cancel" );
						onClicked: {
							PopupUtils.close( formatDialog );
						}
					}
				}
			}
			
			onClicked: {
				PopupUtils.open( formatDialogComponent, this, { "index": ScriptFormatter.getFormat( scriptTA.textDocument, scriptTA.cursorPosition ) } )
			}
		}
		
		Controls.TextArea {
			width: parent.width
			height: parent.height - buttonRow.height - typeButton.height
			//contentWidth: width
			//autoSize: false
			//maximumLineCount: 0
			id: scriptTA
			objectName: "scriptTA"
			//mouseSelectionMode: TextEdit.SelectWords
			textFormat: TextEdit.RichText
			wrapMode: TextEdit.Wrap
			
			/*Component.onCompleted: {
				font.setStyleHint( Qt.Typewriter )
			}*/
			
			Component.onCompleted: {
				ScriptFormatter.setDefaultFontForDocument( textDocument );
				ReportGenerator.setScript( textDocument );
			}
			
			onLineCountChanged: {
				ScriptFormatter.textChanged( textDocument, cursorPosition );
			}
		}
	}
}
