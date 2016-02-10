import QtQuick 2.0
import Ubuntu.Components 1.3

import ninja.theopensource.scriptdragon 1.0 //for any data that can't be attached directly to the QML Notecard object

Rectangle {
	width: parent.width / 2
	height: parent.height / 2
	color: "lightgray"
	border.color: "black"
	Column {
		width: parent.width// *.9
		height: parent.height// *.9
		
		TextField {
			placeholderText: i18n.tr( "Notecard" )
			color: "black"
			width: parent.width
			horizontalAlignment: TextInput.AlignHCenter
			opacity: 0.9
		}
		
		TextArea {
			width: parent.width
			autoSize: false
			maximumLineCount: 0
			opacity: 0.9
			
			color: "black" //This is the color of the text, not of the text area itself
			textFormat: TextEdit.RichText
			
			placeholderText: i18n.tr( "Type here" )
		}
		
		Label {
			text: i18n.tr( "Associativity: " )
		}
	}
}

