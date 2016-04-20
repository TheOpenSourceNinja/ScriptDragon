#include "namegenerator.h"
#include <stdio.h>
#include <QFile>
#include <iostream>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <vector>
#include <random>

NameGenerator::NameGenerator( QObject *parent ) : QObject( parent ) {
	gen = std::mt19937(rd());
	
	{ //Load first names
		QFile theFile( ":/first_names.csv" );
		if( theFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
			
			while( !theFile.atEnd() ) {
				QString line = QString::fromUtf8( theFile.readLine() );
				QStringList theList = line.split( ',' );
				uint_fast32_t number = theList[ 2 ].toULongLong();
				firstNameProbabilities.push_back( number );
				firstNames.push_back( theList[ 1 ] );
				if( theList[ 0 ].compare( "M", Qt::CaseInsensitive ) == 0 ) {
					maleFirstNameProbabilities.push_back(number);
					maleFirstNames.push_back( firstNames.size() - 1 );
				} else if( theList[ 0 ].compare( "F", Qt::CaseInsensitive ) == 0 ) {
					femaleFirstNameProbabilities.push_back(number);
					femaleFirstNames.push_back( firstNames.size() - 1 );
				} else {
					std::cout << "Malformatted line: \"" << line.toStdString() << "\"" << std::endl;
				}
			}
			
			chooseAnyFirstName = std::discrete_distribution< uint_fast32_t >( firstNameProbabilities.begin(), firstNameProbabilities.end() );
			chooseMaleFirstName = std::discrete_distribution< uint_fast32_t >( maleFirstNameProbabilities.begin(), maleFirstNameProbabilities.end() );
			chooseFemaleFirstName = std::discrete_distribution< uint_fast32_t >( femaleFirstNameProbabilities.begin(), femaleFirstNameProbabilities.end() );
		} else {
			std::cerr << "Error opening file \"" << theFile.fileName().toStdString() << "\"" << std::endl;
		}
	}
	
	{ //Load last names
		QFile theFile( ":/last_names.csv" );
		if( theFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
			
			theFile.readLine(); //The first line is a header; skip it
			
			while( !theFile.atEnd() ) {
				QString line = QString::fromUtf8( theFile.readLine() );
				QStringList theList = line.split( ',' );
				uint_fast32_t number = theList[ 2 ].toULongLong();
				lastNameProbabilities.push_back( number );
				
				//Properly capitalize names; the input file has them in all caps
				QString firstLetter = theList[ 0 ].left( 1 ).toUpper();
				QString theRest = theList[ 0 ].right( theList[ 0 ].size() - 1 ).toLower();
				QString result = firstLetter + theRest;
				
				lastNames.push_back( result );
			}
			
			chooseLastName = std::discrete_distribution< uint_fast32_t >( lastNameProbabilities.begin(), lastNameProbabilities.end() );
		} else {
			std::cerr << "Error opening file \"" << theFile.fileName().toStdString() << "\"" << std::endl;
		}
	}
}

QString NameGenerator::getFirstName( gender_t gender ) {
	switch( gender ) {
		case gender_t::MALE: {
			return firstNames[ maleFirstNames[ chooseMaleFirstName( gen ) ] ];
			break;
		}
		case gender_t::FEMALE: {
			return firstNames[ femaleFirstNames[ chooseFemaleFirstName( gen ) ] ];
			break;
		}
		default: {
			return firstNames[ chooseAnyFirstName( gen ) ];
			break;
		}
	}
}

QString NameGenerator::getLastName() {
	return lastNames[ chooseLastName( gen ) ];
}

