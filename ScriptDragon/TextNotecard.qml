import QtQuick 2.0
import Ubuntu.Components 1.3
import QtQuick.Dialogs 1.2
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import ninja.theopensource.scriptdragon 1.0

//-------------------------BEGIN SECTION: COMMON TO ALL NOTECARDS-----------------------------
//This is stuff that should be common to all notecards of all types. If changes are made here, replicate them elsewhere!!

import ninja.theopensource.scriptdragon 1.0 //for any data that can't be attached directly to a QML Notecard object, this gives us access to the C++-defined NotecardData type

Rectangle {
	width: units.gu( 5 * 6 ) //The 5 is from real notecards, which can be 5x7 inches. The *6 multiplier is to make it usable on my computer screen and was found through experimentation.
	height: width * 1.4 //*1.4 reflects the aspect ratio of 5x7 notecards.
	color: "white"
	border.color: "black"
	id: theCard
	property alias text: textArea.text
	property alias title: titleArea.text
	
	property var associatedCharacterId
	property var associatedCharacterName: ""
	
	ColorDialog {
		id: colorDialog
		title: i18n.tr( "Choose a color for this notecard" )
		showAlphaChannel: false
		color: theCard.color
		currentColor: theCard.color
		
		onAccepted: {
			theCard.color = currentColor //The QT documentation says we should use "color", not "currentColor", but I've found that "color" only works every other time whereas "currentColor" works every time.
		}
	}
	
	//note: memory corruption error disappears if the following NotecardData is commented out
	//note 2: memory corruption error happens on Ubuntu 16.04 VM, not 15.10 real hardware
	NotecardData {
		id: data
	}
	
	Column {
		property real sizeMultiplier: .98 //Added because I want some margins for the color to show through. 98% just seems like a good value.
		width: parent.width * sizeMultiplier
		height: parent.height * sizeMultiplier
		anchors.centerIn: parent
		spacing: units.gu( 1 )
		
		Column {
			id: header
			width: parent.width
			spacing: parent.spacing
			
			TextField {
				placeholderText: i18n.tr( "Notecard Title" )
				color: "black"
				width: parent.width
				horizontalAlignment: TextInput.AlignHCenter
				opacity: 0.9
				id: titleArea
			}
			
			Row {
				width: parent.width
				
				Button {
					text: i18n.tr( "color" );
					id: colorButton
					onClicked: {
						colorDialog.open()
					}
				}
				Button {
					text: i18n.tr( "Hold to delete" )
					iconName: "delete"
					color: UbuntuColors.red
					width: parent.width - colorButton.width
					
					onPressAndHold: {
						NotecardManager.removeNotecard( theCard );
						theCard.parent.setChildren();
					}
				}
			}
		}
		
		//-------------------------END SECTION: COMMON TO ALL NOTECARDS-----------------------------
		//-------------------------BEGIN SECTION: SPECIFIC TO THIS NOTECARD TYPE-----------------------------
		//This is stuff that should be common to all notecards of this particular type. 
		
		TextArea {
			width: parent.width
			height: parent.height - header.height - associativity.height
			autoSize: false
			maximumLineCount: 0
			opacity: 0.9
			id: textArea
			
			color: "black" //This is the color of the text, not of the text area itself
			
			placeholderText: i18n.tr( "Type a note here" )
		}
		
		//-------------------------END SECTION: SPECIFIC TO THIS NOTECARD TYPE-----------------------------
		//-------------------------BEGIN SECTION: COMMON TO ALL NOTECARDS-----------------------------
		Row {
			id: associativity
			width: parent.width
			Label {
				text: i18n.tr( "Associativity: " )
			}
			Label {
				text: associatedCharacterName
			}

			Button {
				id: linkButton
				text: i18n.tr( "Link with character" )
				
				onClicked: {
					console.log(charactersTab.characterListModel);
					PopupUtils.open( characterDialogComponent )
				}
				
				Component {
					id: characterDialogComponent
					Dialog {
						id: characterDialog
						title: i18n.tr( "Choose character" )
						
						ListItem.ItemSelector {
							id: selector
							model: charactersTab.characterListModel
							expanded: false
							
							/*onDelegateClicked: {
								console.log( index )
								characterView.setVisibleChild( index )
							}
							
							onSelectedIndexChanged: {
								console.log( selectedIndex )
								characterView.setVisibleChild( selectedIndex )
							}*/
							
						}
						
						Row {
							Button {
								text: i18n.tr( "OK" )
								onClicked: {
									associatedCharacterId = selector.selectedIndex
									associatedCharacterName = charactersTab.characters[ selector.selectedIndex ].name
									console.log(associatedCharacterName)
									onClicked: PopupUtils.close( characterDialog )
								}
							}
							
							Button {
								text: i18n.tr( "Cancel" )
								onClicked: PopupUtils.close( characterDialog )
							}
						}
					}
				}
			}
		}
	}
}
//-------------------------END SECTION: COMMON TO ALL NOTECARDS-----------------------------
