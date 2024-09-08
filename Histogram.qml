import QtQuick
import Gis 1.0
Item {
    Canvas {
        id: histogram
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            var topWordsList = fileManager.topWords;
            if (topWordsList.length === 0)
                return;

            var barWidth = width / topWordsList.length

            for (var i = 0; i < topWordsList.length; i++) {
                var word = topWordsList[i].word;
                var count = topWordsList[i].count.toString();
                var barHeight = (height - 50) * count/topWordsList[0].count

                ctx.fillStyle = "blue"
                ctx.fillRect(i * barWidth, height - barHeight - 25, barWidth - 5, barHeight)

                ctx.fillStyle = "black"
                ctx.font = "8px Verdana"

                ctx.fillText(count, i * barWidth + barWidth / 2 - ctx.measureText(count).width / 2, height - barHeight - 35)
                ctx.font = "12px Verdana"
                ctx.fillText(word, i * barWidth + barWidth / 2 - ctx.measureText(word).width / 2, height - 10)
            }
        }

        Connections {
            target: fileManager
            function onTopWordsChanged() {
                histogram.requestPaint()
            }
        }
    }
}
