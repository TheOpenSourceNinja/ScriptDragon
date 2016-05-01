import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import ninja.theopensource.scriptdragon 1.0

Page {
	
	NameGenerator {
		id: nameGenerator;
	}
	
	property var characterArchetypes: [ i18n.tr( "Anima/Animus" ), i18n.tr( "Eternal Child" ), i18n.tr( "Devil" ), i18n.tr( "God/Goddess" ), i18n.tr( "Great Mother/Father" ), i18n.tr( "Shapeshifter" ), i18n.tr( "Threshold Guardian" ), i18n.tr( "Shadow" ), i18n.tr( "Herald" ), i18n.tr( "Ally" ), i18n.tr( "Mentor" ), i18n.tr( "Hero" ) ] //Mostly taken from Christopher Vogler's "The Writer's Journey", 3rd edition. The rest are from https://en.wikipedia.org/w/index.php?title=Jungian_archetypes&oldid=699271078#Examples . The archetypes listed here must EXACTLY correspond with the descriptions in the archetypeDescriptions variable lower down.
	property int characterArchetypeChosen: 0; //This only gets read when the user clicks the 'Archetype Descriptions' button, part of the new character dialog. Why is it outside the dialog? Because it needs to be written by the newCharacter() function (below).
	
	property var characterName;
	property var characterAge;
	property var characterGender;
	property var characterJob;
	
	//This function is outside of the new character Dialog because it also needs to be accessed by the new character Button
	function newCharacter() {
		
		//Regarding genders: I wanted to reflect modern inclusiveness by including an option other than binary male/female, but also wanted to keep things simple since this is intended to be used very early in the writing process. Having only three options, I feel, is the right thing to do, and "other/nonspecific" gender has a reasonably clear meaning. That said, I'm still considering changing it to something like "gender-fluid" or "non-binary gender".
		var genders = new Array(3);
		genders[ NameGenerator.MALE ] = i18n.tr( "male" );
		genders[ NameGenerator.FEMALE ] = i18n.tr( "female" );
		genders[ NameGenerator.NONSPECIFIC ] = i18n.tr( "other/nonspecific" );
		
		var jobs = [ i18n.tr( "farmer" ), i18n.tr( "doctor" ), i18n.tr( "jobless" ), i18n.tr( "warrior" ), i18n.tr( "bartender" ), i18n.tr( "animal trainer" ), i18n.tr( "toy maker" ), i18n.tr( "entertainer" ), i18n.tr( "police" ), i18n.tr( "cook" ), i18n.tr( "demolitions expert" ), i18n.tr( "artist" ), i18n.tr( "carpenter" ), i18n.tr( "servant" ) ] //Trying to not add too many jobs, but also put in a good variety.
		
		var ages = [ i18n.tr( "baby" ), i18n.tr( "child" ), i18n.tr( "teenager" ), i18n.tr( "young adult" ), i18n.tr( "middle-aged" ), i18n.tr( "old" ) ]
		
		var text = "";
		characterAge = ages[ Math.floor( Math.random() * ages.length ) ];
		text += i18n.tr( "Age: " ) + characterAge + "<br />"
		
		var genderNumber = Math.floor( Math.random() * genders.length )
		characterGender = genders[ genderNumber ];
		text += i18n.tr( "Gender: " ) + characterGender + "<br />"
		
		characterArchetypeChosen = Math.floor( Math.random() * characterArchetypes.length );
		characterArchetype = characterArchetypes[ characterArchetypeChosen ];
		text += i18n.tr( "Archetype: " ) + characterArchetype + "<br />"
		characterJob = jobs[ Math.floor( Math.random() * jobs.length ) ];
		text += i18n.tr( "Job: " ) + characterJob +"<br />"
		
		characterName = newName( genderNumber );
		text += i18n.tr( "Name: " ) + characterName;
		return text;
	}
	
	function newLocation() {
		var environments = [ i18n.tr( "in the desert" ), i18n.tr( "on the beach" ), i18n.tr( "in the forest" ), i18n.tr( "in a big city" ), i18n.tr( "in a small village/town" ), i18n.tr( "near a river/lake" ), i18n.tr( "on or under the ocean" ), i18n.tr( "in outer space" ), i18n.tr( "on a farm" ), i18n.tr( "in a field/plain/grassland" ), i18n.tr( "in a garden" ), i18n.tr( "in ice/snow/tundra" ), i18n.tr( "in a swamp/wetland" ), i18n.tr( "underground" ) ]
		var buildings = [ i18n.tr( "outside" ), i18n.tr( "house" ), i18n.tr( "castle" ), i18n.tr( "skyscraper" ), i18n.tr( "office" ), i18n.tr( "hospital" ), i18n.tr( "restaurant" ), i18n.tr( "ancient/abandoned building" ), i18n.tr( "cave" ), i18n.tr( "vehicle" ), i18n.tr( "fork in the road" ), i18n.tr( "school" ), i18n.tr( "museum" ), i18n.tr( "zoo" ), i18n.tr( "factory" ) ]
		
		var text = "";
		text += buildings[ Math.floor( Math.random() * buildings.length ) ] + " "
		text += environments[ Math.floor( Math.random() * environments.length ) ]
		return text;
	}
	
	function newEvent() {
		var text = i18n.tr( "Characters: " );
		
		var numCharacters = Math.floor( ( Math.random() * Math.min( 5, ( characters.length > 0 ) ? characters.length : 5 ) ) + 1 ); //1-~5 characters seems like a good range
		
		var charactersAlreadyChosen = [];
		
		for( var i = 0; i < numCharacters; ++i ) {
			
			if( characters.length > 0 ) {
				var characterChosen = Math.floor( Math.random() * characters.length );
				while( charactersAlreadyChosen.indexOf( characterChosen ) != -1 ) {
					characterChosen = Math.floor( Math.random() * characters.length );
				}
				
				charactersAlreadyChosen.splice( 0, 0, characterChosen );
				
				text += characters[ characterChosen ].name;
			} else {
				text += newName( NameGenerator.NONSPECIFIC );
			}

			if( i < numCharacters - 2 ) {
				text += ", ";
			} else if( i < numCharacters - 1 && numCharacters > 1 ) {
				text += i18n.tr( ", and " );
			}
		}
		
		text += "<br />";
		
		var activities = [ i18n.tr( "touring/on vacation" ), i18n.tr( "biking" ), i18n.tr( "boating" ), i18n.tr( "climbing" ), i18n.tr( "camping" ), i18n.tr( "fishing" ), i18n.tr( "hiking" ), i18n.tr( "horseback riding" ), i18n.tr( "hunting" ), i18n.tr( "picnicking" ), i18n.tr( "swimming" ), i18n.tr( "wildlife viewing" ), i18n.tr( "taking photos" ), i18n.tr( "hacking computers" ), i18n.tr( "playing a board/card game" ), i18n.tr( "playing sports" ), i18n.tr( "fighting" ), i18n.tr( "having a quiet discussion" ), i18n.tr( "having a not-so-quiet 'discussion'" ), i18n.tr( "crossing a threshold" ), i18n.tr( "doing drugs" ), i18n.tr( "trying to ignore each other" ), i18n.tr( "Confronting a Big Evil Thing" ), i18n.tr( "Searching for a MacGuffin" ), i18n.tr( "doing his/her job", "doing their jobs", numCharacters ), i18n.tr( "firefighting" ) ];
		
		text += i18n.tr( "Location: " );
		
		if( locations.length > 0 ) {
			text += locations[ Math.floor( Math.random() * locations.length ) ].name;
		} else {
			text += newLocation();
		}
		
		text += "<br />";
		
		text += i18n.tr( "Doing: " ) + activities[ Math.floor( Math.random() * activities.length ) ];
		return text
	}
	
	function newName( gender ) {
		var text = nameGenerator.getFirstName( gender );
		text += " ";
		text += nameGenerator.getLastName();
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
			
			Component {
				id: popoverComponent
				
				Popover {
					id: popover
					TextArea {
						id: popoverLabel
						width: parent.width
						readOnly: true
						maximumLineCount: 0
					}
					
					onVisibleChanged: {
						var archetypeDescriptions = [ i18n.tr( "Anima/animus: The inner self, the id, the soul. This character is an unthinking, driving force." ), i18n.tr( "Eternal Child: One who never accepts the responsibilities and freedoms of adulthood." ), i18n.tr( "Devil: A demon, an evil person." ), i18n.tr( "God/Goddess: One who is good, powerful, and probably very arrogant." ), i18n.tr( "Great Mother/Father: A caring person who becomes like a parental figure to everyone they know." ), i18n.tr( "Shapeshifter: One whose role in the story changes, e.g. from ally to enemy or vice versa. May be a trickster." ), i18n.tr( "Threshold Guardian: One who challenges and tests the hero. Passing the test represents a significant change in the hero's life." ), i18n.tr( "Shadow: A looming, pervasive influence. One who follows behind other characters." ), i18n.tr( "Herald: A bringer of (usually good) news, a messenger." ), i18n.tr( "Ally: One who helps the hero, physically or emotionally." ), i18n.tr( "Mentor: A teacher, a wise counselor, an advisor to the hero." ), i18n.tr( "Hero: The main character, the Chosen One, the one who must become the savior" ) ]; //The archetype descriptions here must EXACTLY correspond to the archetypes in the characterArchetypes variable.
						
						popoverLabel.text = archetypeDescriptions[ characterArchetypeChosen ];
					}
				}
			}
			
			Button {
				id: popoverButton;
				text: i18n.tr( "Archetype description" );
				onClicked: PopupUtils.open( popoverComponent, popoverButton );
			}
			
			Button {
				text: i18n.tr( "Generate another" )
				onClicked: {
					dialog.text = newCharacter()
					description.text = ""
				}
			}
			
			Button {
				text: i18n.tr( "Save as character" )
				onClicked: {
					
					charactersPage.addCharacter( characterName, characterAge, characterGender, characterArchetypes[ characterArchetypeChosen ], characterJob, description.text )
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
					dialog.text = newName( NameGenerator.NONSPECIFIC )
					description.text = ""
				}
			}
			
			Button {
				text: i18n.tr( "Generate another (male)" )
				onClicked: {
					dialog.text = newName( NameGenerator.MALE )
					description.text = ""
				}
			}
			
			Button {
				text: i18n.tr( "Generate another (female)" )
				onClicked: {
					dialog.text = newName( NameGenerator.FEMALE )
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
