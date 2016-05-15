import QtQuick 2.0
//import QtQuick.Controls 1.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import ninja.theopensource.scriptdragon 1.0

Page {
	
	property alias storylines: storylineView.children
	property alias storylineListModel: storylineListModel
	
	property var component: Qt.createComponent( "Storyline.qml" );
	
	Connections {
		target: FileSaverAndLoader;
		onAddStorylineName: {
			var status = component.status;
			if( status === Component.Ready ) {
				var obj = component.createObject( storylineView )
				
				if( obj === null ) {
					console.error( i18n.tr( "Error adding storyline." ) )
				} else {
					if( storylineName !== null ) {
						obj.name = storylineName
					}
					
					obj.storylineID = storylineListModel.count
					
					storylineListModel.append( { text:storylineName } )
					
					selector.selectedIndex = storylineListModel.count - 1
				}
			} else if( status === Component.Error ){
				console.error( i18n.tr( "Error adding storyline: " ) + component.errorString() );
			}
		}
	}
	
	function addStoryline( name ) {
		var status = component.status;
		if( status === Component.Ready ) {
			var obj = component.createObject( storylineView )
			
			if( obj === null ) {
				console.error( i18n.tr( "Error creating new storyline." ) )
			} else {
				if( name !== null ) {
					obj.name = name
				}
				
				obj.storylineID = storylineListModel.count
				
				NotecardManager.addNotecard( i18n.tr( "Storyline" ), "", NotecardManager.STORYLINE, obj.storylineID );
				
				storylineListModel.append( { text:name } )
				
				selector.selectedIndex = storylineListModel.count - 1
			}
		} else if( status === Component.Error ){
			console.error( i18n.tr( "Error creating new storyline: " ) + component.errorString() );
		}
	}
	
	function deleteStoryline( indexToRemove ) {
		//console.log( NotecardManager.getNotecardsForStoryline( indexToRemove ) );
		var array = NotecardManager.getNotecardsForStoryline( indexToRemove );
		for( var i = 0; i < array.length; ++i ) {
			console.log( array[ i ] );
			NotecardManager.removeAssociation( array[ i ] );
		}
		
		storylineListModel.remove( indexToRemove )
		storylineView.removeChild( indexToRemove )
		selector.selectedIndex -= 1
	}
	
	
	Column {
		anchors.fill: parent
		
		Column {
			id: header
			width: parent.width
			Row {
				Button {
					text: i18n.tr( "New storyline" )
					
					property int storylineNumber: 0;
					
					onClicked: {
						var name = i18n.tr( "Storyline " ) + storylineNumber;
						storylineNumber++;
						var age = "";
						var gender = "";
						var archetype = "";
						var job = "";
						
						addStoryline( name, age, gender, archetype, job )
					}
				}
				
				Button {
					text: i18n.tr( "Delete storyline" )
					enabled: storylineListModel.count > 0
					
					onClicked: deleteStoryline( selector.selectedIndex );
				}
			}
			
			Rectangle {
				id: spacer //This rectangle is a workaround: something invisible was overlapping the buttons, preventing them from receiving click events.
				width: parent.width
				height: units.gu(2);
			}
			
			ListItem.ItemSelector {
				text: "Choose storyline:"
				id: selector
				model: storylineListModel
				expanded: false
				
				onDelegateClicked: {
					//console.log( index )
					storylineView.setVisibleChild( index )
				}
				
				onSelectedIndexChanged: {
					//console.log( selectedIndex )
					storylineView.setVisibleChild( selectedIndex )
				}
				
				ListModel {
					id: storylineListModel
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
			id: storylineView
			objectName: "storylineView"
			
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
