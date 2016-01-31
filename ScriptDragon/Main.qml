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
	
	Page {
		title: i18n.tr("ScriptDragon")
		
		Column {
			spacing: units.gu(1)
			anchors {
				margins: units.gu(2)
				fill: parent
			}
			
			Label {
				id: label
				objectName: "label"
				
				text: i18n.tr("Hello..")
			}
			
			Button {
				objectName: "button"
				width: parent.width
				
				text: i18n.tr("Tap me!")
				
				onClicked: {
					label.text = i18n.tr("..world!")
				}
			}
		}
	}
}

