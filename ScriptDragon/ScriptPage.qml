import QtQuick 2.0
import Ubuntu.Components 1.3

Page {
	property alias text: scriptTA.text
	TextArea {
		width: parent.width
		height: parent.height
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
