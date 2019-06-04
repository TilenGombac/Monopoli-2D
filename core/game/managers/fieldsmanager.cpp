#include "fieldsmanager.h"

#include "core/game/entity/fields/cardfield.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/cornerfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/game/entity/fields/taxfield.h"

QVector<Field*> FieldsManager::getDefaultFields(QWidget *parent)
{
	QVector<Field*> fields;

	CardField *cardField;
	CompanyField *companyField;
	CornerField *cornerField;
	PropertyField *propertyField;
	TaxField *taxField;

	Action action;

	// Corner Fields

	action.setCollectAction(200);
	cornerField = new CornerField(0, parent);
	cornerField->setImage(QImage(":/img/img/fields/start.png"));
	cornerField->setName("Vzemi");
	cornerField->setAction(action);
	fields.append(cornerField);

	action.setNoAction();
	cornerField = new CornerField(10, parent);
	cornerField->setImage(QImage(":/img/img/fields/jail.png"));
	cornerField->setName("Obisk / Zapor");
	cornerField->setAction(action);
	fields.append(cornerField);

	action.setNoAction();
	cornerField = new CornerField(20, parent);
	cornerField->setImage(QImage(":/img/img/fields/freeParking.png"));
	cornerField->setName("Brezplačno parkiranje");
	cornerField->setAction(action);
	fields.append(cornerField);

	action.setGoToJailAction();
	cornerField = new CornerField(30, parent);
	cornerField->setImage(QImage(":/img/img/fields/goToJail.png"));
	cornerField->setName("Pojdi v zapor");
	cornerField->setAction(action);
	fields.append(cornerField);


	// Tax fields

	// This could probably be done in the tax field class itself...
	action.setPayAction(200);
	taxField = new TaxField(4, parent);
	taxField->setImage(QImage(":/img/img/fields/incomeTax.png"));
	taxField->setName("Davek na dohodek");
	taxField->setPayString("Plačaj");
	taxField->setCost(200);
	taxField->setAction(action);
	fields.append(taxField);

	// This could probably be done in the tax field class itself...
	action.setPayAction(100);
	taxField = new TaxField(38, parent);
	taxField->setImage(QImage(":/img/img/fields/luxuryTax.png"));
	taxField->setName("Davek na dobiček");
	taxField->setPayString("Plačaj");
	taxField->setCost(100);
	taxField->setAction(action);
	fields.append(taxField);


	// Company fields

	action.setNoAction();
	companyField = new CompanyField(5, parent);
	companyField->setImage(QImage(":/img/img/fields/railway.png"));
	companyField->setName("Železniška postaja Jesenice");
	companyField->setPrice(200);
	companyField->setAction(action);
	companyField->setGroupId(1);
	companyField->setRent(25);
	fields.append(companyField);

	action.setNoAction();
	companyField = new CompanyField(15, parent);
	companyField->setImage(QImage(":/img/img/fields/railway.png"));
	companyField->setName("Glavni kolodvor");
	companyField->setPrice(200);
	companyField->setAction(action);
	companyField->setGroupId(1);
	companyField->setRent(25);
	fields.append(companyField);

	action.setNoAction();
	companyField = new CompanyField(25, parent);
	companyField->setImage(QImage(":/img/img/fields/railway.png"));
	companyField->setName("Železniška postaja Zidani Most");
	companyField->setPrice(200);
	companyField->setAction(action);
	companyField->setGroupId(1);
	companyField->setRent(25);
	fields.append(companyField);

	action.setNoAction();
	companyField = new CompanyField(35, parent);
	companyField->setImage(QImage(":/img/img/fields/railway.png"));
	companyField->setName("Železniški terminal Koper");
	companyField->setPrice(200);
	companyField->setAction(action);
	companyField->setGroupId(1);
	companyField->setRent(25);
	fields.append(companyField);

	action.setNoAction();
	companyField = new CompanyField(12, parent);
	companyField->setImage(QImage(":/img/img/fields/electricity.png"));
	companyField->setName("Javna razsvetljava");
	companyField->setPrice(150);
	companyField->setAction(action);
	companyField->setGroupId(4);
	fields.append(companyField);

	action.setNoAction();
	companyField = new CompanyField(28, parent);
	companyField->setImage(QImage(":/img/img/fields/waterworks.png"));
	companyField->setName("Mestni vodovod");
	companyField->setPrice(150);
	companyField->setAction(action);
	companyField->setGroupId(4);
	fields.append(companyField);


	// Card fields

	action.setCommunityChestAction();
	cardField = new CardField(2, parent);
	cardField->setImage(QImage(":/img/img/fields/communityChest.png"));
	cardField->setName("Državna blagajna");
	cardField->setAction(action);
	fields.append(cardField);

	action.setChanceAction();
	cardField = new CardField(7, parent);
	cardField->setImage(QImage(":/img/img/fields/chance.png"));
	cardField->setName("Priložnost");
	cardField->setAction(action);
	fields.append(cardField);

	action.setCommunityChestAction();
	cardField = new CardField(17, parent);
	cardField->setImage(QImage(":/img/img/fields/communityChest.png"));
	cardField->setName("Državna blagajna");
	cardField->setAction(action);
	fields.append(cardField);

	action.setChanceAction();
	cardField = new CardField(22, parent);
	cardField->setImage(QImage(":/img/img/fields/chance.png"));
	cardField->setName("Priložnost");
	cardField->setAction(action);
	fields.append(cardField);

	action.setCommunityChestAction();
	cardField = new CardField(33, parent);
	cardField->setImage(QImage(":/img/img/fields/communityChest.png"));
	cardField->setName("Državna blagajna");
	cardField->setAction(action);
	fields.append(cardField);

	action.setChanceAction();
	cardField = new CardField(36, parent);
	cardField->setImage(QImage(":/img/img/fields/chance.png"));
	cardField->setName("Priložnost");
	cardField->setAction(action);
	fields.append(cardField);


	// Property fields

	action.setNoAction();
	propertyField = new PropertyField(1, parent);
	propertyField->setName("Šobec");
	propertyField->setColor(QColor(134, 76, 56));
	propertyField->setPrice(60);
	propertyField->setAction(action);
	propertyField->setGroupId(0);
	propertyField->setRent(2);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(3, parent);
	propertyField->setName("Fiesa");
	propertyField->setColor(QColor(134, 76, 56));
	propertyField->setPrice(60);
	propertyField->setAction(action);
	propertyField->setGroupId(0);
	propertyField->setRent(4);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(6, parent);
	propertyField->setName("Ljutomerske gorice");
	propertyField->setColor(QColor(172, 220, 239));
	propertyField->setPrice(100);
	propertyField->setAction(action);
	propertyField->setGroupId(2);
	propertyField->setRent(6);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(8, parent);
	propertyField->setName("Haloze");
	propertyField->setColor(QColor(172, 220, 239));
	propertyField->setPrice(100);
	propertyField->setAction(action);
	propertyField->setGroupId(2);
	propertyField->setRent(6);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(9, parent);
	propertyField->setName("Goriška Brda");
	propertyField->setColor(QColor(172, 220, 239));
	propertyField->setPrice(120);
	propertyField->setAction(action);
	propertyField->setGroupId(2);
	propertyField->setRent(8);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(11, parent);
	propertyField->setName("Bogenšperk");
	propertyField->setColor(QColor(197, 56, 131));
	propertyField->setPrice(140);
	propertyField->setAction(action);
	propertyField->setGroupId(3);
	propertyField->setRent(14);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(13, parent);
	propertyField->setName("Mokrice");
	propertyField->setColor(QColor(197, 56, 131));
	propertyField->setPrice(140);
	propertyField->setAction(action);
	propertyField->setGroupId(3);
	propertyField->setRent(10);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(14, parent);
	propertyField->setName("Otočec");
	propertyField->setColor(QColor(197, 56, 131));
	propertyField->setPrice(160);
	propertyField->setAction(action);
	propertyField->setGroupId(3);
	propertyField->setRent(12);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(16, parent);
	propertyField->setName("Vogel");
	propertyField->setColor(QColor(236, 139, 43));
	propertyField->setPrice(180);
	propertyField->setAction(action);
	propertyField->setGroupId(5);
	propertyField->setRent(10);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(18, parent);
	propertyField->setName("Rogla");
	propertyField->setColor(QColor(236, 139, 43));
	propertyField->setPrice(180);
	propertyField->setAction(action);
	propertyField->setGroupId(5);
	propertyField->setRent(14);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(19, parent);
	propertyField->setName("Krajnska Gora");
	propertyField->setColor(QColor(236, 139, 43));
	propertyField->setPrice(200);
	propertyField->setAction(action);
	propertyField->setGroupId(5);
	propertyField->setRent(16);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(21, parent);
	propertyField->setName("Terme Čatež");
	propertyField->setColor(QColor(218, 36, 40));
	propertyField->setPrice(220);
	propertyField->setAction(action);
	propertyField->setGroupId(6);
	propertyField->setRent(18);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(23, parent);
	propertyField->setName("Radenci");
	propertyField->setColor(QColor(218, 36, 40));
	propertyField->setPrice(220);
	propertyField->setAction(action);
	propertyField->setGroupId(6);
	propertyField->setRent(18);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(24, parent);
	propertyField->setName("Moravske toplice");
	propertyField->setColor(QColor(218, 36, 40));
	propertyField->setPrice(240);
	propertyField->setAction(action);
	propertyField->setGroupId(6);
	propertyField->setRent(20);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(26, parent);
	propertyField->setName("Logarska dolina");
	propertyField->setColor(QColor(255, 240, 3));
	propertyField->setPrice(260);
	propertyField->setAction(action);
	propertyField->setGroupId(7);
	propertyField->setRent(22);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(27, parent);
	propertyField->setName("Trenta");
	propertyField->setColor(QColor(255, 240, 3));
	propertyField->setPrice(260);
	propertyField->setAction(action);
	propertyField->setGroupId(7);
	propertyField->setRent(22);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(29, parent);
	propertyField->setName("Lipica");
	propertyField->setColor(QColor(255, 240, 3));
	propertyField->setPrice(280);
	propertyField->setAction(action);
	propertyField->setGroupId(7);
	propertyField->setRent(24);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(31, parent);
	propertyField->setName("Cerkniško jezero");
	propertyField->setColor(QColor(19, 167, 88));
	propertyField->setPrice(300);
	propertyField->setAction(action);
	propertyField->setGroupId(8);
	propertyField->setRent(26);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(32, parent);
	propertyField->setName("Bled");
	propertyField->setColor(QColor(19, 167, 88));
	propertyField->setPrice(300);
	propertyField->setAction(action);
	propertyField->setGroupId(8);
	propertyField->setRent(26);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(34, parent);
	propertyField->setName("Bohinj");
	propertyField->setColor(QColor(19, 167, 88));
	propertyField->setPrice(320);
	propertyField->setAction(action);
	propertyField->setGroupId(8);
	propertyField->setRent(28);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(37, parent);
	propertyField->setName("Piran");
	propertyField->setColor(QColor(0, 103, 164));
	propertyField->setPrice(350);
	propertyField->setAction(action);
	propertyField->setGroupId(9);
	propertyField->setRent(35);
	fields.append(propertyField);

	action.setNoAction();
	propertyField = new PropertyField(39, parent);
	propertyField->setName("Portorož");
	propertyField->setColor(QColor(0, 103, 164));
	propertyField->setPrice(400);
	propertyField->setAction(action);
	propertyField->setGroupId(9);
	propertyField->setRent(50);
	fields.append(propertyField);

	return fields;
}
