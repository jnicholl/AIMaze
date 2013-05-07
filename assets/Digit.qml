import bb.cascades 1.0

Container {
    id: numberContainer
    property int base
    property int number
    property int oldNumber
    clipContentToBounds: true
    maxHeight: 55
    layout: AbsoluteLayout {
    }
    ImageView {
        translationY: (numberContainer.number) * -numberContainer.maxHeight
        animations: [
            TranslateTransition {
                objectName: "animation"
                fromY: (numberContainer.oldNumber) * - numberContainer.maxHeight
                toY: (numberContainer.number) * - numberContainer.maxHeight
                duration: 500
            }
        ]
        imageSource: "asset:///images/numbers.png"
    }
}
