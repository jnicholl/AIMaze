// Navigation pane project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QtCore/QVariantList>
#include <bb/cascades/OrientationSupport>
#include <bb/device/DisplayInfo>

namespace bb {
	namespace cascades {
		class Application;
		class ListView;
		class NavigationPane;
		class Page;
	}
}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int screenWidth READ screenWidth WRITE setScreenWidth
            NOTIFY screenWidthChanged)
    Q_PROPERTY(int screenHeight READ screenHeight WRITE setScreenHeight
			NOTIFY screenHeightChanged)
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {}

    Q_SLOT void startLevel(const QVariantList &indexPath);
    Q_SLOT void onOrientationAboutToChange(bb::cascades::UIOrientation::Type);

    int screenWidth() const { return m_screenWidth; }
    void setScreenWidth(int width) {
    	if (m_screenWidth != width) {
    		m_screenWidth = width;
    		emit screenWidthChanged();
    	}
    }
    int screenHeight() const { return m_screenHeight; }
    void setScreenHeight(int height) {
    	if (m_screenHeight != height) {
    		m_screenHeight = height;
    		emit screenHeightChanged();
    	}
    }

    Q_SLOT void onPixelSizeChanged( QSize pixelSize ) {
    	setScreenWidth(pixelSize.width());
    	setScreenHeight(pixelSize.height());
    }

signals:
	void screenWidthChanged();
	void screenHeightChanged();

private:
    bb::cascades::ListView *m_levelList;
    bb::cascades::Page *m_gamePage;
    bb::cascades::NavigationPane *m_navigationPane;

    int m_screenWidth;
    int m_screenHeight;
    bb::device::DisplayInfo m_display;
};

#endif /* ApplicationUI_HPP_ */
