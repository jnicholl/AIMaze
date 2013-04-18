import bb.cascades 1.0

Container {
    // Screen size
    property int screenWidth: 1280
    property int screenHeight: 768

    // Size of gameplay area during action phase
    property int playAreaWidth: 1160
    property int playAreaHeight: 640
    
    // Size of gameplay area during compile phase
    property int compileMapWidth: playAreaWidth
    property int compileMapHeight: 512
    
    // Size of bars during action phase
    property int sidebarWidth: screenWidth - playAreaWidth
    property int queueHeight: screenHeight - playAreaHeight
    property int compileFunctionHeight: screenHeight - compileMapHeight;

    // Size of command buttons
    property int cmdWidth: sidebarWidth - sidebarPadding
    property int cmdHeight: sidebarWidth - sidebarPadding
    
    // Padding between bar edge and command button
    property int sidebarPadding: 40
    
    // Padding between queue items
    property int queueItemPadding: 30
    
    // Padding between sidebar items
    property int itemPadding: 20
    
    // Used for pausebutton on L-series
    property int backButtonHeight: 82
    
    
}

