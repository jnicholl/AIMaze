// Navigation pane project template
#include "applicationui.hpp"

#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/ListView>
#include <bb/cascades/Page>
#include <bb/cascades/PaneProperties>
#include <bb/cascades/GroupDataModel>
#include <bb/data/JsonDataAccess>
#include <bb/cascades/Container>
#include <bb/cascades/Label>

#include <xutility>


using namespace bb::cascades;
using namespace bb::device;
using namespace bb::data;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
, m_levelList(0)
, m_gamePage(0)
, m_screenWidth(0)
, m_screenHeight(0)
{
	setScreenWidth(m_display.pixelSize().width());
	setScreenHeight(m_display.pixelSize().height());
	connect(&m_display, SIGNAL(pixelSizeChanged(QSize)),
			this, SLOT(onPixelSizeChanged(QSize)));

	// create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("_app", this);

    // create root object for the UI
    m_navigationPane = qml->createRootObject<NavigationPane>();
    m_levelList = m_navigationPane->findChild<ListView*>("listView");
    if (!m_levelList) qFatal("Failed to find level list in QML");

    // set created root object as a scene
    app->setScene(m_navigationPane);

//    {
//        // load JSON data from file to QVariant
//        JsonDataAccess jda;
//        QVariantList lst = jda.load("app/native/assets/levels.json").toList();
//        if (jda.hasError()) {
//            bb::data::DataAccessError error = jda.error();
//            qDebug() << "JSON loading error: " << error.errorType() << ": " << error.errorMessage();
//        }
//        else {
//            qDebug() << "JSON data loaded OK!";
//            GroupDataModel *m = new GroupDataModel();
//            m->setSortingKeys(QStringList() << "id"); //
//            // insert the JSON data to model
//            m->insertList(lst);
//            // make the model flat
//            m->setGrouping(ItemGrouping::None);
//            m_levelList->setDataModel(m);
//        }
//    }

    QObject::connect(
    		OrientationSupport::instance(),
    		SIGNAL(orientationAboutToChange(bb::cascades::UIOrientation::Type)),
    		this,
    		SLOT(onOrientationAboutToChange(bb::cascades::UIOrientation::Type)));
}

void ApplicationUI::startLevel(const QVariantList &indexPath)
{
	Container *minimap = 0;
	if (!m_gamePage) {
		QmlDocument *qml = QmlDocument::create("asset:///Game.qml").parent(this);
		qml->setContextProperty("_app", this);

		m_gamePage = qml->createRootObject<Page>();
		minimap = m_gamePage->findChild<Container*>("minimap");
	} else {
		minimap = m_gamePage->findChild<Container*>("minimap");
		if (minimap) {
			minimap->removeAll();
		}
	}

	QVariantMap levelInfo = m_levelList->dataModel()->data(indexPath).toMap();
	QString levelPath = levelInfo["level"].toString();
	JsonDataAccess jda;
	qDebug() << "Attempting to load " << levelPath;
	QVariantMap levelData = jda.load(levelPath).toMap();
	if (jda.hasError()) {
		bb::data::DataAccessError error = jda.error();
		qDebug() << "JSON loading error: " << error.errorType() << ": " << error.errorMessage();
	} else {
		qDebug() << "JSON data loaded OK!";

		if (!minimap) qFatal("Couldn't find minimap?");

		int minimapSize = std::min(minimap->preferredWidth(), minimap->preferredHeight());
		int cols = levelData["width"].toInt();
		int rows = levelData["height"].toInt();
		// FIXME: aspect ratio distortion
		int colSize = minimapSize / cols;
		int rowSize = minimapSize / rows;

		for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				Label *control = Label::create(QString("%1,%2").arg(j).arg(i))
					.layoutProperties(AbsoluteLayoutProperties::create().position(j*colSize, i*rowSize))
					.preferredSize(colSize, rowSize);
				minimap->add(control);
			}
		}
	}

	m_navigationPane->push(m_gamePage);
}

void ApplicationUI::onOrientationAboutToChange(UIOrientation::Type orientation)
{
	if ((orientation == UIOrientation::Landscape && m_screenWidth < m_screenHeight)
			|| (orientation == UIOrientation::Portrait && m_screenWidth > m_screenHeight)) {
		int tmp = m_screenWidth;
		setScreenWidth(m_screenHeight);
		setScreenHeight(tmp);
	}
}
