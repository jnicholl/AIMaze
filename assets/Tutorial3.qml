import bb.cascades 1.0

Container {
    id: tutorial
    property variant dimensions: Dimensions {
    }
    property int state: 0

    signal hideTutorial
    signal showRun
    signal showCompile

    preferredWidth: dimensions.screenWidth
    preferredHeight: dimensions.screenHeight
    layout: DockLayout {
    }

    function showScreen() {
    }

    Container {
        visible: state == 0
        preferredWidth: dimensions.screenWidth
        preferredHeight: dimensions.screenHeight
        background: Color.create(0, 0.3, 0.3, 0.7)
        layout: DockLayout {
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            textStyle.textAlign: TextAlign.Center
            multiline: true
            text: "EMUU Training - Part 3

Glad you figured it out... because I've got some new orders from above. I'm being reassigned - something to do with programming phones to shoot lasers. So I guess you'll have to take over here. From now on, you'll be working with the real thing in our test facilities."
        }
    }

    Container {
        visible: state == 1
        preferredWidth: dimensions.screenWidth
        preferredHeight: dimensions.screenHeight
        background: Color.create(0, 0.3, 0.3, 0.7)
        layout: DockLayout {
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            textStyle.textAlign: TextAlign.Center
            multiline: true
            text: "EMUU Training - Part 3

One more thing, there's some glitch in how the robot works. The robot has limited power, right? But when you put an action in function memory, it doesn't seem to cost any power. Except if you call another function. Do some testing, we need to know how this works."
        }
    }

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Bottom
        bottomPadding: dimensions.itemPadding

        Button {
            text: "Previous"
            onClicked: {
                state --;
            }
            enabled: state > 0
            rightMargin: dimensions.queueItemPadding
        }

        Button {
            id: skip
            text: "Skip"
            onClicked: {
                tutorial.hideTutorial();
            }
            enabled: state < 1
            rightMargin: dimensions.queueItemPadding
        }

        Button {
            id: next
            text: state < 1 ? "Next" : "Done"
            onClicked: {
                state ++;
                if (state > 1) tutorial.hideTutorial();
            }
            rightMargin: dimensions.queueItemPadding
        }
    }
}