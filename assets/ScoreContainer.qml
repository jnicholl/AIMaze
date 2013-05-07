import bb.cascades 1.0

Container {
    id: scoreContainer
    horizontalAlignment: HorizontalAlignment.Center
    verticalAlignment: VerticalAlignment.Center
    property int number
    property int oldNumber
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    Digit {
        base: 100000
        opacity: scoreContainer.number >= base ? 1.0 : 0.0
        number: Math.floor(scoreContainer.number / base) % 10
        oldNumber: Math.floor(scoreContainer.oldNumber / base) % 10
    }
    Digit {
        base: 10000
        opacity: scoreContainer.number >= base?1.0:0.0
        number: Math.floor(scoreContainer.number / base) % 10
        oldNumber: Math.floor(scoreContainer.oldNumber / base) % 10
    }
    Digit {
        base: 1000
        opacity: scoreContainer.number >= base ? 1.0 : 0.0
        number: Math.floor(scoreContainer.number / base) % 10
        oldNumber: Math.floor(scoreContainer.oldNumber / base) % 10
    }
    Digit {
        base: 100
        opacity: scoreContainer.number >= base ? 1.0 : 0.0
        number: Math.floor(scoreContainer.number / base) % 10
        oldNumber: Math.floor(scoreContainer.oldNumber / base) % 10
    }
    Digit {
        base: 10
        opacity: scoreContainer.number >= base ? 1.0 : 0.0
        number: Math.floor(scoreContainer.number / base) % 10
        oldNumber: Math.floor(scoreContainer.oldNumber / base) % 10
    }
    Digit {
        number: scoreContainer.number % 10
        oldNumber: scoreContainer.oldNumber % 10
    }
}

