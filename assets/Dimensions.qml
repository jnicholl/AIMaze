import bb.cascades 1.0

Container {
    property int playAreaWidth: 1160
    property int playAreaHeight: 640
    property int screenWidth: 1280
    property int screenHeight: 768
    property int sidebarWidth: screenWidth - playAreaWidth
    property int queueHeight: screenHeight - playAreaHeight
    property int cmdWidth: sidebarWidth - sidebarPadding
    property int cmdHeight: sidebarWidth - sidebarPadding
    property int sidebarPadding: 40
    property int queueItemPadding: 30
    property int itemPadding: 20
    property int backButtonHeight: 82
}

