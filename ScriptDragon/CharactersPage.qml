import QtQuick 2.0
//import QtQuick.Controls 1.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import ninja.theopensource.scriptdragon 1.0

Page {
	
	property alias characters: characterView.children
	property alias characterListModel: characterListModel
	
	Column {
		anchors.fill: parent
		
		Column {
			id: header
			width: parent.width
			Row {
				Button {
					text: i18n.tr( "New character" )
					
					property var component: Qt.createComponent( "Character.qml" );
					
					function addCharacter( /*string*/newCharacterName ) {
						var status = component.status;
						if( status === Component.Ready ) {
							var obj = component.createObject( characterView )
							
							if( obj === null ) {
								console.error( i18n.tr( "Error creating new character." ) )
							} else {
								if( newCharacterName !== null ) {
									obj.initialName = newCharacterName
								}
								
								obj.characterID = characterListModel.count
								
								NotecardManager.addNotecard( i18n.tr( "Physical Characteristics" ), i18n.tr( "Age: \nDistiguishing Features: \nHair: \nEyes: \nHeight: \nWeight: "), NotecardManager.CHARACTER, obj.characterID );
								NotecardManager.addNotecard( i18n.tr( "Personality" ), i18n.tr( "Key character traits: \nVirtues & vices: \nLikes & dislikes: " ), NotecardManager.CHARACTER, obj.characterID );
								NotecardManager.addNotecard( i18n.tr( "Motivation" ), i18n.tr( "Goal: \nPlan to achieve the goal: " ), NotecardManager.CHARACTER, obj.characterID );
								NotecardManager.addNotecard( i18n.tr( "Background" ), i18n.tr( "Family history: \nPersonal history: " ), NotecardManager.CHARACTER, obj.characterID );
								NotecardManager.addNotecard( i18n.tr( "Relation to other characters" ), "", NotecardManager.CHARACTER, obj.characterID );
							}
						} else if( status === Component.Error ){
							console.error( i18n.tr( "Error creating new character: " ) + component.errorString() );
						}
					}
					
					onClicked: {
						var name = "Unnamed Character";
						addCharacter( name )
						
						characterListModel.append( { text:name } )
						
						selector.selectedIndex = characterListModel.count - 1
					}
				}
				
				Button {
					text: i18n.tr( "Delete character" )
					
					onClicked: {
						characterListModel.remove( selector.selectedIndex )
						characterView.removeChild( selector.selectedIndex )
						selector.selectedIndex -= 1
					}
				}
			}
			
			ListItem.ItemSelector {
				text: "Choose character:"
				id: selector
				model: characterListModel
				expanded: false
				
				onDelegateClicked: {
					//console.log( index )
					characterView.setVisibleChild( index )
				}
				
				onSelectedIndexChanged: {
					//console.log( selectedIndex )
					characterView.setVisibleChild( selectedIndex )
				}
				
				ListModel {
					id: characterListModel
					onDataChanged: {
						//console.log("onDataChanged() called");
						
						//These next four lines are a workaround: the selector's text only gets updated if we change models
						var selected = selector.selectedIndex
						selector.model = fakeModel
						selector.model = this
						selector.selectedIndex = selected
					}
				}
				
				ListModel {
					id: fakeModel
					//This is a workaround: the selector's text only gets updated if we change models
				}
			}
			
		}
		
		
		Item {
			width: parent.width
			height: parent.height - header.height
			id: characterView
			
			function setVisibleChild( childNumber ) {
				for( var i = 0; i < children.length; ++i ) {
					children[ i ].visible = false
				}
				
				children[ childNumber ].visible = true
			}
			
			function removeChild( childNumber ) {
				children[ childNumber ].destroy();
			}
		}
	}
}
