import bb.cascades 1.0

Container {
    id: tutorial
    property variant dimensions: Dimensions {
    }
    property int state: 1

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
            text: "EMUU Training - Part 2

I guess you did alright with the sim. We found something new, poking around in the guts of the thing. It has a little extra memory for storing stuff to do later. One of the programmers clearly has no imagination, he just called them functions."
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
            text: "EMUU Training - Part 2

So we changed the simulator a bit. What, you think we'd let you touch the real thing? Let's see how well you do with a little more memory. You can switch between functions by tapping the M. I'm only giving you access to one function, I'm sure you can figure out how to use it."
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