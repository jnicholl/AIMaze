import bb.cascades 1.0

Container {
    property variant dimensions: Dimensions {
    }
    property int index: 0
    property alias imageSource: view.imageSource
    verticalAlignment: VerticalAlignment.Center
    preferredWidth: dimensions.cmdWidth
    preferredHeight: dimensions.cmdHeight
    background: normal.imagePaint
    rightMargin: dimensions.queueItemPadding

    ImageView {
        id: view
    }

    gestureHandlers: [
        TapHandler {
            onTapped: {
                _app.removeQueuedCommand(index);
            }
        }
    ]

    attachedObjects: [
        ImagePaintDefinition {
            id: normal
            imageSource: "asset:///images/border.png"
        }
    ]
}
