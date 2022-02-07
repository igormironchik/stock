
import QtQuick 2.7
import QtQuick.Controls 2.2

Item {
	id: root

    ScrollView {
        id: scroll
        anchors.fill: parent
        padding: minimumCtrlHeight / 4

        Label {
            width: root.width - scroll.padding * 2
            wrapMode: Text.WordWrap
            text: "Copyright 2011 The LibYuv Project Authors. All rights reserved.\n\
\n\
Redistribution and use in source and binary forms, with or without\
modification, are permitted provided that the following conditions are\
met:\n\
\n\
  * Redistributions of source code must retain the above copyright\
    notice, this list of conditions and the following disclaimer.\n\
\n\
  * Redistributions in binary form must reproduce the above copyright\
    notice, this list of conditions and the following disclaimer in\
    the documentation and/or other materials provided with the\
    distribution.\n\
\n\
  * Neither the name of Google nor the names of its contributors may\
    be used to endorse or promote products derived from this software\
    without specific prior written permission.\n\
\n\
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\
\"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT\
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,\
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT\
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY\
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
        }
    }
 }
