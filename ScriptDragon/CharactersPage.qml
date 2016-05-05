import QtQuick 2.0
//import QtQuick.Controls 1.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import ninja.theopensource.scriptdragon 1.0

Page {
	
	property alias characters: characterView.children
	property alias characterListModel: characterListModel
	
	property var component: Qt.createComponent( "Character.qml" );
	
	function addCharacter( name, age, gender, archetype, job, personality ) {
		var status = component.status;
		if( status === Component.Ready ) {
			var obj = component.createObject( characterView )
			
			if( obj === null ) {
				console.error( i18n.tr( "Error creating new character." ) )
			} else {
				if( name !== null ) {
					obj.name = name
				}
				
				obj.characterID = characterListModel.count
				
				NotecardManager.addNotecard( i18n.tr( "Physical Characteristics" ), i18n.tr( "Age: ") + age + "\n" + i18n.tr( "Gender: ") + gender + "\n" + i18n.tr( "Distiguishing Features: \nHair: \nEyes: \nHeight: \nWeight: "), NotecardManager.CHARACTER, obj.characterID );
				NotecardManager.addNotecard( i18n.tr( "Personality" ), i18n.tr( "Virtues & vices: \nLikes & dislikes: \nArchetype: " ) + archetype + "\n" + i18n.tr( "Job: " ) + job + "\n" + i18n.tr( "Other notes: " ) + personality, NotecardManager.CHARACTER, obj.characterID );
				NotecardManager.addNotecard( i18n.tr( "Motivation" ), i18n.tr( "Goal: \nPlan to achieve the goal: " ), NotecardManager.CHARACTER, obj.characterID );
				NotecardManager.addNotecard( i18n.tr( "Background" ), i18n.tr( "Family history: \nPersonal history: " ), NotecardManager.CHARACTER, obj.characterID );
				NotecardManager.addNotecard( i18n.tr( "Relation to other characters" ), "", NotecardManager.CHARACTER, obj.characterID );
				
				characterListModel.append( { text:name } )
				
				selector.selectedIndex = characterListModel.count - 1
			}
		} else if( status === Component.Error ){
			console.error( i18n.tr( "Error creating new character: " ) + component.errorString() );
		}
	}
	
	function deleteCharacter( indexToRemove ) {
		//console.log( NotecardManager.getNotecardsForCharacter( indexToRemove ) );
		var array = NotecardManager.getNotecardsForCharacter( indexToRemove );
		for( var i = 0; i < array.length; ++i ) {
			console.log( array[ i ] );
			NotecardManager.removeAssociation( array[ i ] );
		}
		
		characterListModel.remove( indexToRemove )
		characterView.removeChild( indexToRemove )
		selector.selectedIndex -= 1
	}
	
	
	Column {
		anchors.fill: parent
		
		Column {
			id: header
			width: parent.width
			Row {
				Button {
					text: i18n.tr( "New character" )
					
					onClicked: {
						var name = "Unnamed Character";
						var age = "";
						var gender = "";
						var archetype = "";
						var job = "";
						
						addCharacter( name, age, gender, archetype, job )
					}
				}
				
				Button {
					text: i18n.tr( "Delete character" )
					
					onClicked: deleteCharacter( selector.selectedIndex );
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
