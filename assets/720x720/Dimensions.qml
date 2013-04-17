import bb.cascades 1.0

Container {
    property int playAreaWidth: 640
    property int playAreaHeight: 640
    property int screenWidth: 720
    property int screenHeight: 720
    property int sidebarWidth: screenWidth-playAreaWidth
    property int queueHeight: screenHeight - playAreaHeight
    property int cmdWidth: sidebarWidth - 20
    property int cmdHeight: sidebarWidth - 20
    property int sidebarPadding: 20
    property int backButtonHeight: 62
}

