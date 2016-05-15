import QtQuick 2.0
import Ubuntu.Components 1.3
import QtQuick.Dialogs 1.2 as QtDialogs
import Ubuntu.Components.Popups 1.0 as UbuntuDialogs
import ninja.theopensource.scriptdragon 1.0 //Ignore the warning about this "QML module" not being found

Page {
	title: i18n.tr( "Welcome" )
	id: thePage
	
	property var currentFileURL: "";
	
	Connections {
		target: FileSaverAndLoader
		onFileLoaded: {
			//SettingsManager.setValue( "lastFileLoaded", fileURL );
			thePage.currentFileURL = fileURL;
		}
		
		onFileSaved: {
			thePage.currentFileURL = fileURL;
		}
	}
	
	//Used by the file open & save dialogs
	property var fileNameFilters: [
		"ScriptDragon files (*.scriptdragon)",
		"All files (*)"
	]
	
	QtDialogs.FileDialog {
		id: openDialog
		title: i18n.tr( "Choose a file to open" )
		folder: shortcuts.documents //Reference: https://doc.qt.io/qt-5/qml-qtquick-dialogs-filedialog.html#shortcuts-prop
		selectExisting: true
		selectMultiple: false
		
		nameFilters: fileNameFilters
		
		onAccepted: {
			FileSaverAndLoader.load( fileUrl ); //If selectMultiple were true, we would use the plural fileUrls
		}
		
		onRejected: {
			//label.text = i18n.tr( "Open dialog cancelled" )
		}
	}
	
	QtDialogs.FileDialog {
		id: saveDialog
		title: i18n.tr( "Choose a file to save to" )
		folder: shortcuts.documents
		
		selectExisting: false //From the docs: "Whether only existing files or directories can be selected... Setting this property to false implies that the dialog is for naming a file to which to save something... therefore selectMultiple must be false."
		selectMultiple: false //If this were true then the user couldn't input a file name
		
		nameFilters: fileNameFilters
		
		onAccepted: {
			FileSaverAndLoader.save( fileUrl );
		}
		
		onRejected: {
			//label.text = i18n.tr( "Save dialog cancelled" )
		}
	}
	
	Column {
		spacing: units.gu(1)
		anchors {
			margins: units.gu(2)
			fill: parent
		}
		
		Component {
			id: savePopoverComponent
			UbuntuDialogs.Popover {
				id: savePopover
				
				Label {
					text: "Saved"
				}
			}
		}
		
		Column {
			id: buttons;
			width: parent.width
			
			Button {
				objectName: "newButton"
				width: parent.width
				id: newButton
				iconName: "document-new"
				text: i18n.tr( "New file" )
				
				
				
				onClicked: {
					console.log( "New File button clicked" );
					
					characters = [];
					characterListModel.clear();
					NotecardManager.removeAllNotecards();
					locations = [];
					//locationListModel.clear();//TODO: Uncomment this when the time comes
					script = ""
					report = ""
					currentFileURL = ""
				}
			}
			
			Button {
				objectName: "openButton"
				width: parent.width
				id: openButton
				iconName: "document-open"
				text: i18n.tr( "Open latest (hold to open a different file)" )
				
				onClicked: {
					var fileURL = SettingsManager.getValue( "lastFileLoaded" );
					if( fileURL !== undefined && fileURL !== "" ) {
						FileSaverAndLoader.load( fileURL );
					} else {
						openDialog.visible = true;
					}
				}
				
				onPressAndHold: {
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
					if( currentFileURL !== undefined && currentFileURL !== "" ) {
						FileSaverAndLoader.save( currentFileURL );
					} else {
						saveDialog.visible = true;
					}
					
					PopupUtils.open( savePopoverComponent, savePopover );
				}
				
				onPressAndHold: {
					saveDialog.visible = true
				}
			}
		}
		
		TextArea {
			id: tour
			width: parent.width
			height: parent.height - buttons.height;
			readOnly: true
			//autoSize: true
			maximumLineCount: 0
			activeFocusOnPress: false
			cursorVisible: false
			selectByMouse: false //note: this appears not to do anything because of activeFocusOnPress and/or cursorVisible being false. readOnly being true might also be a factor. Setting it just in case.
			textFormat:Text.RichText;
			
			text: i18n.tr(
				"Welcome to ScriptDragon. ScriptDragon is an app for screenwriters.<br />" +
				"The different sections of the app can be accessed from the menu at the top of the screen:" +
				"<ul>" +
					"<li>Notecards: The app's main feature, notecards are where you write out all your ideas no matter how crazy.</li>" +
					"<li>Characters: Displays notecards associated with specific characters.</li>" +
					"<li>Locations: Displays notecards associated with specific locations.</li>" +
					"<li>Storylines: Displays notecards associated with specific storylines.</li>" +
					"<li>Random generators: Need inspiration? Look here!</li>" +
					"<li>Script: Where you write the script.</li>" +
					"<li>Reports: Statistics etc. generated based on the script.</li>" +
					"<li>Welcome: This screen.</li>" +
				"</ul>"
			)
		}
	}
}

