import bb.cascades 1.0

Container {
    property int playAreaWidth: 600
    property int playAreaHeight: 600
    property int screenWidth: 720
    property int screenHeight: 720
    property int sidebarWidth: screenWidth - playAreaWidth
    property int queueHeight: screenHeight - playAreaHeight
    property int cmdWidth: sidebarWidth - sidebarPadding
    property int cmdHeight: sidebarWidth - sidebarPadding
    property int sidebarPadding: 40
    property int queueItemPadding: 30
    property int itemPadding: 20
    property int backButtonHeight: 62
}

