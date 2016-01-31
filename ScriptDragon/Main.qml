import QtQuick 2.4
import Ubuntu.Components 1.3

/*!
	\brief MainView with a Label and Button elements.
*/

MainView {
	// objectName for functional testing purposes (autopilot-qt5)
	objectName: "mainView"
	
	// Note! applicationName needs to match the "name" field of the click manifest
	applicationName: "scriptdragon.dearingj"
	
	/* 
	 This property enables the application to change orientation 
	 when the device is rotated. The default is false.
	*/
	//automaticOrientation: true
	
	
	width: units.gu(100)
	height: units.gu(75)
	
	Tabs {
		id: tabs
		
		Tab {
			id: welcomeTab
			title: i18n.tr( "Welcome" )
			
			Page {
				
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
				}
			}
		}
		
		Tab {
			id: notecardsTab
			title: i18n.tr( "Notecards" )
		}
		
		Tab {
			id: scriptTab
			title: i18n.tr( "Script" )
			
			Page {
				TextArea {
					width: parent.width
					height: parent.height
					//contentWidth: width
					autoSize: false
					maximumLineCount: 0
					
					color: "black" //This is the color of the text, not of the text area itself
					font.family: "Courier" //TODO: Find a way to use alternate font families if Courier is unavailable
					font.pointSize: 15
					textFormat: TextEdit.RichText
					
					placeholderText: i18n.tr( "Type here" )
				}
			}
		}
		
		Tab {
			id: reportsTab
			title: i18n.tr( "Reports" )
		}
	}
}

