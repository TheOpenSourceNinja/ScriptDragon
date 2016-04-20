#ifndef NAMEGENERATOR_H
#define NAMEGENERATOR_H

#include <QObject>

class NameGenerator : public QObject
{
		Q_OBJECT
	public:
		explicit NameGenerator(QObject *parent = 0);
		
		enum class gender_t: uint_fast8_t {
			MALE,
			FEMALE,
			NONSPECIFIC
		};
		Q_ENUMS( gender_t );
		
		Q_INVOKABLE QString getFirstName( gender_t gender );
		
		Q_INVOKABLE QString getLastName();
		
	signals:
		
	public slots:
		
	private:
		std::random_device rd;
		std::mt19937 gen;
		std::discrete_distribution< uint_fast32_t > chooseAnyFirstName;
		std::discrete_distribution< uint_fast32_t > chooseMaleFirstName;
		std::discrete_distribution< uint_fast32_t > chooseFemaleFirstName;
		std::discrete_distribution< uint_fast32_t > chooseLastName;
		std::vector< uint_fast32_t > femaleFirstNames; //We save memory and possibly loading time by having the gender-specific first name vectors contain only indices into the gender-neutral first name vector
		std::vector< uint_fast32_t > maleFirstNames;
		std::vector< QString > firstNames;
		std::vector< QString > lastNames;
		std::vector< uint_fast32_t > femaleFirstNameProbabilities;
		std::vector< uint_fast32_t > maleFirstNameProbabilities;
		std::vector< uint_fast32_t > firstNameProbabilities;
		std::vector< uint_fast32_t > lastNameProbabilities;
};

#endif // NAMEGENERATOR_H
