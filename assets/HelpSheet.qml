import bb.cascades 1.0

Sheet {
    id: helpSheet
    peekEnabled: false
    Page {
        Container {
            id: root
            property variant dimensions: Dimensions {
            }
            layout: DockLayout{}
            ScrollView {
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Center
                preferredHeight: root.dimensions.screenHeight - 100
                content: Container {                
		            ImageView {
		                verticalAlignment: VerticalAlignment.Top
		                horizontalAlignment: HorizontalAlignment.Center
		                imageSource: "asset:///images/help.png"
		            }
		            Label {
		                text: 
"The Electronic Mobile Utility Unit (EMUU) is a short-range remote-control robot with very basic commands and rather limited power and memory storage.

EMUU has a command set of three actions, move forward, turn left, and turn right. Additional modules may be available for specific compiled functions (F1 to F3). Functions are compiled and stored in read-only memory on the EMUU itself.

Actions are added to a command queue, and are executed at a fixed rate. The robot operates on a fixed clock cycle, using up the same amount of power per clock tick. This means that the robot's power level will decrease whether or not it has executed an action.

Functions should be used whenever possible because the actions taken inside a function do not cost any power, with the one exception of calling another function."
		                multiline: true
		            }
		        }
            }
            Button {
                id: closeButton
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Fill
                                text: "Close"
                onClicked: {
                    helpSheet.close();
                }
            }
        }
    }
}
