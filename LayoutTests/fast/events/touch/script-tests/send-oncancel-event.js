description("Tests that the cancel touch event is sent correctly.");

var touchX = 25;
var touchY = 25;

var cancelEvent = null;

function touchcancelHandler() {
    shouldBeEqualToString("event.type", "touchcancel");
    cancelEvent = event.changedTouches[0];
    shouldBeNonNull("cancelEvent");
    shouldBe("cancelEvent.pageX", touchX.toString());
    shouldBe("cancelEvent.pageY", touchY.toString());
    if (window.layoutTestController) {
        layoutTestController.notifyDone();
    }
}
    
if (window.layoutTestController)
    window.layoutTestController.waitUntilDone();

if (window.eventSender) {
    document.addEventListener("touchcancel", touchcancelHandler, false);
    eventSender.addTouchPoint(touchX, touchY);
    eventSender.touchStart();
    eventSender.cancelTouchPoint(0);
    eventSender.touchCancel();
} else
    debug("This test requires DumpRenderTree.");

var successfullyParsed = true;
