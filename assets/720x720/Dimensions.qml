import bb.cascades 1.0

Container {
    // Screen size
    property int screenWidth: 720
    property int screenHeight: 720

    // Size of gameplay area during action phase
    property int playAreaWidth: 600
    property int playAreaHeight: 600

    // Size of gameplay area during compile phase
    property int compileMapWidth: playAreaWidth
    property int compileMapHeight: 512
    
    // Size of bars
    property int sidebarWidth: screenWidth - playAreaWidth
    property int queueHeight: screenHeight - playAreaHeight
    property int compileFunctionHeight: screenHeight - compileMapHeight;

    // Size of command buttons
    property int cmdWidth: sidebarWidth - sidebarPadding
    property int cmdHeight: sidebarWidth - sidebarPadding

    // Padding between bar edge and command button
    property int sidebarPadding: 30

    // Padding between queue items
    property int queueItemPadding: 30

    // Padding between sidebar items
    property int itemPadding: 10

    // Not used
    property int backButtonHeight: 62
}

