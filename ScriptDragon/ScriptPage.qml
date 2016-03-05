import QtQuick 2.0
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0

Page {
	property alias text: scriptTA.text
	Column {
		width: parent.width
		height: parent.height
		
		Row {
			id: buttonRow
			
			
			//TODO: Convert all these buttons to a drop-down, pop-up, or similar.
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
			}
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
