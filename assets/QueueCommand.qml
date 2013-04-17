import bb.cascades 1.0

Container {
    property variant dimensions: Dimensions {
    }
    property int index: 0
    property alias text: label.text
    verticalAlignment: VerticalAlignment.Center
    preferredWidth: dimensions.cmdWidth
    preferredHeight: dimensions.cmdHeight
    background: Color.Gray
    rightMargin: dimensions.sidebarPadding

    Label {
        id: label
        text: "1"
    }

    gestureHandlers: [
        TapHandler {
            onTapped: {
                _app.removeQueuedCommand(index);
            }
        }
    ]
}
