import QtQuick 2.0
import Ubuntu.Components 1.3

Page {
	title: i18n.tr( "Welcome" )
	
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
				label.text = i18n.tr( "\"Open\" unimplemented" )
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
				label.text = i18n.tr( "\"Save as\" unimplemented")
			}
		}
		
		Label { //todo: would this be better as a non-editable text area? Might have automatic text wrapping.
			id: tourLabel
			objectName: "tourLabel"
			
			text: i18n.tr("This is where I will introduce the app's basic features. Experienced users will know to use the tabs menu or the buttons to go straight to whatever.")
		}
	}
}

