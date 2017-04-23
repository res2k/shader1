/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

import QtQuick 2.0

Item {
    id: imageView

    property int __imageWidth: 100
    property int __imageHeight: 100

    function updateImageDimensions()
    {
        var imgW = image.sourceSize.width, imgH = image.sourceSize.height;

        if ((imgW > width) || (imgH > height))
        {
            var scale1 = Math.min (width / imgW, 1.0);
            var scale2 = Math.min (height / imgH, 1.0);
            var scale = Math.min (scale1, scale2);
            imgW = Math.floor (imgW * scale);
            imgH = Math.floor (imgH * scale);
            console.log (width, height, scale1, scale2, imgW, imgH);
        }

        __imageWidth = imgW;
        __imageHeight = imgH;
    }

    onWidthChanged: updateImageDimensions();
    onHeightChanged: updateImageDimensions();

    Image {
        id: image
        fillMode: Image.PreserveAspectFit
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: imageLocation.url
        width: __imageWidth
        height: __imageHeight
        visible: false
        asynchronous: true
        layer.enabled: true

        onStatusChanged: {
            if (status == Image.Ready) imageView.updateImageDimensions();
        }
    }
    ShaderEffect {
        anchors.fill: image
        property variant src: image
        vertexShader: shaderSource.vertexProgram
        fragmentShader: shaderSource.fragmentProgram

        onLogChanged: {
            console.log("ShaderEffect status:", status, "log:", log);
        }
    }
    Item {
        Rectangle {
            color: "#c0ffffff"
            anchors.fill: statusText
            anchors.margins: -4
            radius: 4
        }
        Text {
            function statusTextStr() {
                switch (image.status) {
                case Image.Error:   return "Error loading image";
                case Image.Loading: return "Loading... " + (Math.floor(image.progress*100)) + "%";
                }
                return "";
            }

            id: statusText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: statusTextStr();
            font.pixelSize: 12
        }

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        visible: statusText.text !== ""
    }
}
