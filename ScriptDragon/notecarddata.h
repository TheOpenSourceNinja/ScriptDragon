#ifndef NOTECARDDATA_H
#define NOTECARDDATA_H

#include <QObject>
#include <QColor>

class NotecardData : public QObject {
		Q_OBJECT
		Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
	public:
		explicit NotecardData(QObject *parent = 0);
		
		void setColor( QColor newColor );
		
		QColor getColor();
		
	signals:
		void colorChanged();
		
	public slots:
		
	private:
		QColor color;
};

#endif // NOTECARDDATA_H
