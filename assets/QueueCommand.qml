import bb.cascades 1.0

Container {
    id: commandContainer
    property variant dimensions: Dimensions {
    }
    property int index: 0
    property alias imageSource: view.imageSource
    preferredWidth: dimensions.cmdWidth
    preferredHeight: dimensions.cmdHeight
    background: normal.imagePaint
    rightMargin: dimensions.queueItemPadding
    layoutProperties: AbsoluteLayoutProperties {
        positionX: dimensions.firstQueueItemX + index * (dimensions.cmdWidth + dimensions.queueItemPadding)
        positionY: (dimensions.queueHeight - dimensions.cmdHeight) / 2
    }
    translationX: dimensions.playAreaWidth

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
    
    animations: [
        TranslateTransition {
            id: translateAnimation
            objectName: "translateAnimation"
            easingCurve: StockCurve.Linear
            toX: dimensions.playAreaWidth - _app.offset * (dimensions.cmdWidth + dimensions.queueItemPadding)
            duration: 500
        }
    ]

    attachedObjects: [
        ImagePaintDefinition {
            id: normal
            imageSource: "asset:///images/border.png"
        },
        ImplicitAnimationController {
            propertyName: "translationX"
            enabled: false
        }
    ]
    
    onCreationCompleted: {
        _app.moveLeft.connect(moveLeft);
    }
}
