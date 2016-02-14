import QtQuick 2.0
import Ubuntu.Components 1.3
import QtQuick.Dialogs 1.2

import ninja.theopensource.scriptdragon 1.0 //for any data that can't be attached directly to a QML Notecard object, this gives us access to the C++-defined NotecardData type

Rectangle {
	width: units.gu( 5 * 6 ) //The 5 is from real notecards, which can be 5x7 inches. The *6 multiplier is to make it usable on my computer screen and was found through experimentation.
	height: width * 1.4 //*1.4 reflects the aspect ratio of 5x7 notecards.
	color: "lightgray"
	border.color: "black"
	id: theCard

	ColorDialog {
		id: colorDialog
		title: i18n.tr( "Choose a color for this notecard" )
		showAlphaChannel: false
		color: theCard.color
		currentColor: theCard.color

		onAccepted: {
			console.log( "You chose the color " + currentColor ) //The QT documentation says we should use "color", not "currentColor", but I've found that "color" only works every other time whereas "currentColor" works every time.
			theCard.color = currentColor
		}
	}

	//note: memory corruption error disappears if the following NotecardData is commented out
	//note 2: memory corruption error happens on Ubuntu 16.04 VM, not 15.10 real hardware
	NotecardData {
		id: data
	}

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

		Button {
			text: i18n.tr( "color" );
			onClicked: {
				colorDialog.open()
			}
		}

		Label {
			text: i18n.tr( "Associativity: \n\n\n" )
		}

		Row {
			width: parent.width

			property int numButtons: 2

			Button {
				text: i18n.tr( "Hold to delete" )
				iconName: "delete"
				color: UbuntuColors.red
				width: parent.width / parent.numButtons

				onPressAndHold: {
					theCard.destroy(); //Note: Qt5 docs don't mention destroy(); they do mention deleteLater() but that didn't work for me
				}
			}
			Button {
				width: parent.width / parent.numButtons
				text: i18n.tr( "Another button" )
			}
		}
	}
}

