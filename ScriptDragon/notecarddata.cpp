#include "notecarddata.h"

NotecardData::NotecardData(QObject *parent) : QObject(parent) {
	setColor( QColor( "lightgray" ) );
}

void NotecardData::setColor( QColor newColor ) {
	if( color != newColor ) {
		color = newColor;
		emit colorChanged();
	}
}

QColor NotecardData::getColor() {
	return color;
}
