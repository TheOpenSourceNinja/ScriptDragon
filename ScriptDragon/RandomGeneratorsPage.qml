import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Page {
	
	//This function is outside of the new character Dialog because it also needs to be accessed by the new character Button
	function newCharacter() {
		
		//Regarding genders: I wanted to reflect modern inclusiveness by including an option other than binary male/female, but also wanted to keep things simple since this is intended to be used very early in the writing process. Having only three options, I feel, is the right thing to do, and "other/nonspecific" gender has a reasonably clear meaning. That said, I'm still considering changing it to something like "gender-fluid" or "non-binary gender".
		var genders = [ i18n.tr( "male" ), i18n.tr( "female" ), i18n.tr( "other/nonspecific" ) ]
		
		var archetypes = [ i18n.tr( "Shapeshifter" ), i18n.tr( "Threshold Guardian" ), i18n.tr( "Trickster" ), i18n.tr( "Shadow" ), i18n.tr( "Herald" ), i18n.tr( "Ally" ), i18n.tr( "Mentor" ), i18n.tr( "Hero" ) ] //Taken from Christopher Vogler's "The Writer's Journey", 3rd edition.
		
		var jobs = [ i18n.tr( "farmer" ), i18n.tr( "doctor" ), i18n.tr( "jobless" ), i18n.tr( "warrior" ), i18n.tr( "bartender" ), i18n.tr( "animal trainer" ), i18n.tr( "toy maker" ), i18n.tr( "entertainer" ), i18n.tr( "police" ), i18n.tr( "cook" ), i18n.tr( "demolitions expert" ), i18n.tr( "artist" ), i18n.tr( "carpenter" ), i18n.tr( "servant" ) ] //Trying to not add too many jobs, but also put in a good variety.
		
		var ages = [ i18n.tr( "baby" ), i18n.tr( "child" ), i18n.tr( "teenager" ), i18n.tr( "young adult" ), i18n.tr( "middle-aged" ), i18n.tr( "old" ) ]
		
		var text = "";
		text += i18n.tr( "Age: " ) + ages[ Math.floor( Math.random() * ages.length ) ] + ", "
		text += i18n.tr( "Gender: " ) + genders[ Math.floor( Math.random() * genders.length ) ] + ", "
		text += i18n.tr( "Archetype: " ) + archetypes[ Math.floor( Math.random() * archetypes.length ) ] + ", "
		text += i18n.tr( "Job: " ) + jobs[ Math.floor( Math.random() * jobs.length ) ]
		return text;
	}
	
	Component {
		id: characterDialogComponent
		
		Dialog {
			id: dialog
			title: i18n.tr( "New character" )
			
			function newNotecard() {
				var notecardText = text + "\n" + description.text;
				notecardsPage.addNotecard( notecardText, i18n.tr( "New Character" ) );
			}
			
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
					newNotecard()
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
				//PopupUtils.open( locationDialog )
			}
		}
		
		Button {
			text: i18n.tr( "New event" )
			onClicked: {
				//PopupUtils.open( eventDialog )
			}
		}
		
		Button {
			text: i18n.tr( "New name" )
			onClicked: {
				//PopupUtils.open( nameDialog )
			}
		}
	}
}
