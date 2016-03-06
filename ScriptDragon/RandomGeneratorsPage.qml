import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Page {
	
	//This function is outside of the new character Dialog because it also needs to be accessed by the new character Button
	function newCharacter() {
		
		//Regarding genders: I wanted to reflect modern inclusiveness by including an option other than binary male/female, but also wanted to keep things simple since this is intended to be used very early in the writing process. Having only three options, I feel, is the right thing to do, and "other/nonspecific" gender has a reasonably clear meaning. That said, I'm still considering changing it to something like "gender-fluid" or "non-binary gender".
		var genders = [ i18n.tr( "male" ), i18n.tr( "female" ), i18n.tr( "other/nonspecific" ) ]
		
		var archetypes = [ i18n.tr( "Anima/Animus" ), i18n.tr( "Eternal Child" ), i18n.tr( "Devil" ), i18n.tr( "God/Goddess" ), i18n.tr( "Great Mother/Father" ), i18n.tr( "Shapeshifter" ), i18n.tr( "Threshold Guardian" ), i18n.tr( "Trickster" ), i18n.tr( "Shadow" ), i18n.tr( "Herald" ), i18n.tr( "Ally" ), i18n.tr( "Mentor" ), i18n.tr( "Hero" ) ] //Mostly taken from Christopher Vogler's "The Writer's Journey", 3rd edition. The rest are from https://en.wikipedia.org/w/index.php?title=Jungian_archetypes&oldid=699271078#Examples
		
		var jobs = [ i18n.tr( "farmer" ), i18n.tr( "doctor" ), i18n.tr( "jobless" ), i18n.tr( "warrior" ), i18n.tr( "bartender" ), i18n.tr( "animal trainer" ), i18n.tr( "toy maker" ), i18n.tr( "entertainer" ), i18n.tr( "police" ), i18n.tr( "cook" ), i18n.tr( "demolitions expert" ), i18n.tr( "artist" ), i18n.tr( "carpenter" ), i18n.tr( "servant" ) ] //Trying to not add too many jobs, but also put in a good variety.
		
		var ages = [ i18n.tr( "baby" ), i18n.tr( "child" ), i18n.tr( "teenager" ), i18n.tr( "young adult" ), i18n.tr( "middle-aged" ), i18n.tr( "old" ) ]
		
		var text = "";
		text += i18n.tr( "Age: " ) + ages[ Math.floor( Math.random() * ages.length ) ] + ", "
		text += i18n.tr( "Gender: " ) + genders[ Math.floor( Math.random() * genders.length ) ] + ", "
		text += i18n.tr( "Archetype: " ) + archetypes[ Math.floor( Math.random() * archetypes.length ) ] + ", "
		text += i18n.tr( "Job: " ) + jobs[ Math.floor( Math.random() * jobs.length ) ]
		return text;
	}
	
	function newLocation() {
		var environments = [ i18n.tr( "in the desert" ), i18n.tr( "on the beach" ), i18n.tr( "in the forest" ), i18n.tr( "in a big city" ), i18n.tr( "in a small village/town" ), i18n.tr( "near a river/lake" ), i18n.tr( "on or under the ocean" ), i18n.tr( "in outer space" ), i18n.tr( "on a farm" ), i18n.tr( "in a field/plain/grassland" ), i18n.tr( "in a garden" ), i18n.tr( "in ice/snow/tundra" ), i18n.tr( "in a swamp/wetland" ), i18n.tr( "underground" ) ]
		var buildings = [ i18n.tr( "outside" ), i18n.tr( "house" ), i18n.tr( "castle" ), i18n.tr( "skyscraper" ), i18n.tr( "office" ), i18n.tr( "hospital" ), i18n.tr( "restaurant" ), i18n.tr( "ancient/abandoned building" ), i18n.tr( "cave" ), i18n.tr( "vehicle" ), i18n.tr( "a fork in the road" ), i18n.tr( "school" ), i18n.tr( "museum" ), i18n.tr( "zoo" ), i18n.tr( "factory" ) ]
		
		var text = "";
		text += buildings[ Math.floor( Math.random() * buildings.length ) ] + " "
		text += environments[ Math.floor( Math.random() * environments.length ) ]
		return text;
	}
	
	function newEvent() {
		var text = "Character 1 and Character 2 meet in Location and do Activity"
		return text
	}
	
	function newName() {
		var text = "Luke Starkiller"
		return text
	}
	
	Component {
		id: characterDialogComponent
		
		Dialog {
			id: dialog
			title: i18n.tr( "New character" )
			
			TextArea {
				id: description
				placeholderText: i18n.tr( "Describe the character's personality" )
			}
			
			Button {
				text: i18n.tr( "Generate another" )
				onClicked: {
					dialog.text = newCharacter()
					description.text = ""
				}
			}
			
			Button {
				text: i18n.tr( "Save as notecard" )
				onClicked: {
					var notecardText = text + "\n" + description.text;
					notecardsPage.addNotecard( notecardText, i18n.tr( "New character" ) );
					dialog.visible = false
				}
			}
			
			Button {
				text: i18n.tr( "Cancel" )
				onClicked: PopupUtils.close( dialog )
			}
		}
	}
	
	Component {
		id: locationDialogComponent
		
		Dialog {
			id: dialog
			title: i18n.tr( "New location" )
			
			function newNotecard() {
				var notecardText = text + "\n" + description.text;
				notecardsPage.addNotecard( notecardText, i18n.tr( "New Location" ) );
			}
			
			TextArea {
				id: description
				placeholderText: i18n.tr( "Describe the location" )
			}
			
			Button {
				text: i18n.tr( "Generate another" )
				onClicked: {
					dialog.text = newLocation()
					description.text = ""
				}
			}
			
			Button {
				text: i18n.tr( "Save as notecard" )
				onClicked: {
					var notecardText = text + "\n" + description.text;
					notecardsPage.addNotecard( notecardText, i18n.tr( "New location" ) );
					dialog.visible = false
				}
			}
			
			Button {
				text: i18n.tr( "Cancel" )
				onClicked: PopupUtils.close( dialog )
			}
		}
	}
	
	Component {
		id: eventDialogComponent
		
		Dialog {
			id: dialog
			title: i18n.tr( "New event" )
			
			TextArea {
				id: description
				placeholderText: i18n.tr( "Describe the action" )
			}
			
			Button {
				text: i18n.tr( "Generate another" )
				onClicked: {
					dialog.text = newEvent()
					description.text = ""
				}
			}
			
			Button {
				text: i18n.tr( "Save as notecard" )
				onClicked: {
					var notecardText = text + "\n" + description.text;
					notecardsPage.addNotecard( notecardText, i18n.tr( "New event" ) );
					dialog.visible = false
				}
			}
			
			Button {
				text: i18n.tr( "Cancel" )
				onClicked: PopupUtils.close( dialog )
			}
		}
	}
	
	Component {
		id: nameDialogComponent
		
		Dialog {
			id: dialog
			title: i18n.tr( "New name" )
			
			TextArea {
				id: description
				placeholderText: i18n.tr( "Note which character this name belongs to" )
			}
			
			Button {
				text: i18n.tr( "Generate another" )
				onClicked: {
					dialog.text = newName()
					description.text = ""
				}
			}
			
			Button {
				text: i18n.tr( "Save as notecard" )
				onClicked: {
					var notecardText = text + "\n" + description.text;
					notecardsPage.addNotecard( notecardText, i18n.tr( "New name" ) );
					dialog.visible = false
				}
			}
			
			Button {
				text: i18n.tr( "Cancel" )
				onClicked: PopupUtils.close( dialog )
			}
		}
	}
	
	Column {
		Button {
			text: i18n.tr( "New character" )
			onClicked: {
				PopupUtils.open( characterDialogComponent, this, { text: newCharacter() } )
			}
		}
		
		Button {
			text: i18n.tr( "New location" )
			onClicked: {
				PopupUtils.open( locationDialogComponent, this, { text: newLocation() } )
			}
		}
		
		Button {
			text: i18n.tr( "New event" )
			onClicked: {
				PopupUtils.open( eventDialogComponent, this, { text: newEvent() } )
			}
		}
		
		Button {
			text: i18n.tr( "New name" )
			onClicked: {
				PopupUtils.open( nameDialogComponent, this, { text: newName() } )
			}
		}
	}
}
