import bb.cascades 1.0
   
Container {
    id: sidebar
    
    signal forwardPressed
    signal leftPressed
    signal rightPressed
    signal functionPressed(int func)
    signal viewFunctionsPressed
    
    property int functionCount: 4
    property bool showViewFunctions: false

    SidebarCommand {
        id: cmdForward
        ImageButton {
            defaultImageSource: "asset:///images/forward.png"
            pressedImageSource: "asset:///images/forwardp.png"
            onClicked: {
                sidebar.forwardPressed();
            }
        }
    }

    SidebarCommand {
        id: cmdLeft
        ImageButton {
            defaultImageSource: "asset:///images/left.png"
            pressedImageSource: "asset:///images/leftp.png"
            onClicked: {
                sidebar.leftPressed();
            }
        }
    }

    SidebarCommand {
        id: cmdRight
        ImageButton {
            defaultImageSource: "asset:///images/right.png"
            pressedImageSource: "asset:///images/rightp.png"
            onClicked: {
                sidebar.rightPressed();
            }
        }
    }

    SidebarCommand {
        id: cmdF1
        visible: functionCount > 1
        ImageButton {
            defaultImageSource: "asset:///images/f1.png"
            pressedImageSource: "asset:///images/f1p.png"
            onClicked: {
                sidebar.functionPressed(1);
            }
        }
    }

    SidebarCommand {
        id: cmdF2
        visible: functionCount > 2
        ImageButton {
            defaultImageSource: "asset:///images/f2.png"
            pressedImageSource: "asset:///images/f2p.png"
            onClicked: {
                sidebar.functionPressed(2);
            }
        }
    }

    SidebarCommand {
        id: cmdF3
        visible: functionCount > 3
        ImageButton {
            defaultImageSource: "asset:///images/f3.png"
            pressedImageSource: "asset:///images/f3p.png"
            onClicked: {
                sidebar.functionPressed(3);
            }
        }
    }

    SidebarCommand {
        id: cmdViewFunctions
        visible: functionCount > 1 && showViewFunctions
        ImageButton {
            defaultImageSource: "asset:///images/viewfunctions.png"
            pressedImageSource: "asset:///images/viewfunctionsp.png"
            onClicked: {
                sidebar.viewFunctionsPressed();
            }
        }
    }
}