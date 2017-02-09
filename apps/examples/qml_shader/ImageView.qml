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
    id: item1
    Image {
        id: image
        fillMode: Image.PreserveAspectFit
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: "images/lena.png"
        width: sourceSize.width > parent.width ? parent.width : sourceSize.width
        height: sourceSize.height > parent.height ? parent.height : sourceSize.height
        visible: false
        layer.enabled: true
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
}
