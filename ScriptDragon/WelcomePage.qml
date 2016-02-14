import QtQuick 2.0
import Ubuntu.Components 1.3
import QtQuick.Dialogs 1.2

Page {
	title: i18n.tr( "Welcome" )

	//Used by the file open & save dialogs
	property var fileNameFilters: [
		"ScriptDragon files (*.scriptdragon)",
		"All files (*)"
	]

	FileDialog {
		id: openDialog
		title: i18n.tr( "Choose a file to open" )
		folder: shortcuts.documents //Reference: https://doc.qt.io/qt-5/qml-qtquick-dialogs-filedialog.html#shortcuts-prop
		selectExisting: true
		selectMultiple: false

		nameFilters: fileNameFilters

		onAccepted: {
			label.text = fileUrl //If selectMultiple were true, we would use the plural fileUrls
		}

		onRejected: {
			label.text = i18n.tr( "Open dialog cancelled" )
		}
	}

	FileDialog {
		id: saveDialog
		title: i18n.tr( "Choose a file to save to" )
		folder: shortcuts.documents
		selectExisting: false
		selectMultiple: false

		nameFilters: fileNameFilters

		onAccepted: {
			label.text = fileUrl
		}

		onRejected: {
			label.text = i18n.tr( "Save dialog cancelled" )
		}
	}

	Column {
		spacing: units.gu(1)
		anchors {
			margins: units.gu(2)
			fill: parent
		}

		Label {
			id: label
			objectName: "label"

			text: i18n.tr("Hello world!")
		}

		Button {
			objectName: "newButton"
			width: parent.width
			id: newButton
			iconName: "document-new"
			text: i18n.tr( "New file" )

			onClicked: {
				label.text = i18n.tr( "\"New file\" button clicked" )
			}
		}

		Button {
			objectName: "openButton"
			width: parent.width
			id: openButton
			iconName: "document-open"
			text: i18n.tr( "Open latest (hold to open a different file)" )

			onClicked: {
				label.text = i18n.tr( "\"Open latest\" button clicked" )
			}

			onPressAndHold: {
				label.text = i18n.tr( "\"Open\" was chosen" )
				openDialog.visible = true //Setting visible to true is equivalent to calling openDialog.open()
			}
		}

		Button {
			objectName: "saveButton"
			width: parent.width
			id: saveButton
			iconName: "document-save"
			text: i18n.tr("Save file (hold to save as new file)")

			onClicked: {
				label.text = i18n.tr( "Save button clicked" )
			}

			onPressAndHold: {
				label.text = i18n.tr( "\"Save as\" was chosen")
				saveDialog.visible = true
			}
		}

		TextArea {
			id: tour
			width: parent.width
			readOnly: true
			autoSize: true
			maximumLineCount: 0
			activeFocusOnPress: false
			cursorVisible: false
			selectByMouse: false //note: this appears not to do anything because of activeFocusOnPress and/or cursorVisible being false. readOnly being true might also be a factor. Setting it just in case.

			text: i18n.tr("This is where I will introduce the app's basic features. Experienced users will know to use the tabs menu or the buttons to go straight to whatever.")
		}
	}
}

