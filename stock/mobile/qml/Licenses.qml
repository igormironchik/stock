
import QtQuick 2.7
import QtQuick.Controls 2.2

Item {
	id: root

    ScrollView {
        id: scroll
        anchors.fill: parent
        padding: minimumCtrlHeight / 4


        Column{
            width: root.width - scroll.padding * 2

            Label {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                onLinkActivated: (link) => Qt.openUrlExternally(link)
                text: "<p><b>Qt License</b>\n</p>\
\n\
<p>   GNU LESSER GENERAL PUBLIC LICENSE\n</p>\
<p>   Version 3, 29 June 2007\n</p>\
\n\
<p>Copyright (C) 2007 Free Software Foundation, Inc. <a href=\"https://fsf.org/\">&lt;https://fsf.org/&gt;</a> \
Everyone is permitted to copy and distribute verbatim copies \
of this license document, but changing it is not allowed.\n</p>\
\n\
\n\
<p>  This version of the GNU Lesser General Public License incorporates \
the terms and conditions of version 3 of the GNU General Public \
License, supplemented by the additional permissions listed below.\n</p>\
\n\
<p>  <b>0.</b> Additional Definitions.\n</p>\
\n\
<p>  As used herein, \"this License\" refers to version 3 of the GNU Lesser \
General Public License, and the \"GNU GPL\" refers to version 3 of the GNU \
General Public License.\n</p>\
\n\
<p>  \"The Library\" refers to a covered work governed by this License, \
other than an Application or a Combined Work as defined below.\n</p>\
\n\
<p>  An \"Application\" is any work that makes use of an interface provided \
by the Library, but which is not otherwise based on the Library. \
Defining a subclass of a class defined by the Library is deemed a mode \
of using an interface provided by the Library.\n</p>\
\n\
<p>  A \"Combined Work\" is a work produced by combining or linking an \
Application with the Library.  The particular version of the Library \
with which the Combined Work was made is also called the \"Linked \
Version\".\n</p>\
\n\
<p>  The \"Minimal Corresponding Source\" for a Combined Work means the \
Corresponding Source for the Combined Work, excluding any source code \
for portions of the Combined Work that, considered in isolation, are \
based on the Application, and not on the Linked Version.\n</p>\
\n\
<p>  The \"Corresponding Application Code\" for a Combined Work means the \
object code and/or source code for the Application, including any data \
and utility programs needed for reproducing the Combined Work from the \
Application, but excluding the System Libraries of the Combined Work.\n</p>\
\n\
<p>  <b>1.</b> Exception to Section 3 of the GNU GPL.\n</p>\
\n\
<p>  You may convey a covered work under sections 3 and 4 of this License \
without being bound by section 3 of the GNU GPL.\n</p>\
\n\
<p>  <b>2.</b> Conveying Modified Versions.\n</p>\
\n\
<p>  If you modify a copy of the Library, and, in your modifications, a \
facility refers to a function or data to be supplied by an Application \
that uses the facility (other than as an argument passed when the \
facility is invoked), then you may convey a copy of the modified \
version:\n</p>\
\n\
<p>   <b>a)</b> under this License, provided that you make a good faith effort to \
   ensure that, in the event an Application does not supply the \
   function or data, the facility still operates, and performs \
   whatever part of its purpose remains meaningful, or\n</p>\
\n\
<p>   <b>b)</b> under the GNU GPL, with none of the additional permissions of \
   this License applicable to that copy.\n</p>\
\n\
<p>  <b>3.</b> Object Code Incorporating Material from Library Header Files.\n</p>\
\n\
<p>  The object code form of an Application may incorporate material from \
a header file that is part of the Library.  You may convey such object \
code under terms of your choice, provided that, if the incorporated \
material is not limited to numerical parameters, data structure \
layouts and accessors, or small macros, inline functions and templates \
(ten or fewer lines in length), you do both of the following:\n</p>\
\n\
<p>   <b>a)</b> Give prominent notice with each copy of the object code that the \
   Library is used in it and that the Library and its use are \
   covered by this License.\n</p>\
\n\
<p>   <b>b)</b> Accompany the object code with a copy of the GNU GPL and this license \
   document.\n</p>\
\n\
<p>  <b>4.</b> Combined Works.\n</p>\
\n\
<p>  You may convey a Combined Work under terms of your choice that, \
taken together, effectively do not restrict modification of the \
portions of the Library contained in the Combined Work and reverse \
engineering for debugging such modifications, if you also do each of \
the following:\n</p>\
\n\
<p>   <b>a)</b> Give prominent notice with each copy of the Combined Work that \
   the Library is used in it and that the Library and its use are \
   covered by this License.\n</p>\
\n\
<p>   <b>b)</b> Accompany the Combined Work with a copy of the GNU GPL and this license \
   document.\n</p>\
\n\
<p>   <b>c)</b> For a Combined Work that displays copyright notices during \
   execution, include the copyright notice for the Library among \
   these notices, as well as a reference directing the user to the \
   copies of the GNU GPL and this license document.\n</p>\
\n\
<p>   <b>d)</b> Do one of the following:\n</p>\
\n\
<p>       <b>0)</b> Convey the Minimal Corresponding Source under the terms of this \
       License, and the Corresponding Application Code in a form \
       suitable for, and under terms that permit, the user to \
       recombine or relink the Application with a modified version of \
       the Linked Version to produce a modified Combined Work, in the \
       manner specified by section 6 of the GNU GPL for conveying \
       Corresponding Source.\n</p>\
\n\
<p>       <b>1)</b> Use a suitable shared library mechanism for linking with the \
       Library.  A suitable mechanism is one that (a) uses at run time \
       a copy of the Library already present on the user's computer \
       system, and (b) will operate properly with a modified version \
       of the Library that is interface-compatible with the Linked \
       Version.\n</p>\
\n\
<p>   <b>e)</b> Provide Installation Information, but only if you would otherwise \
   be required to provide such information under section 6 of the \
   GNU GPL, and only to the extent that such information is \
   necessary to install and execute a modified version of the \
   Combined Work produced by recombining or relinking the \
   Application with a modified version of the Linked Version. (If \
   you use option 4d0, the Installation Information must accompany \
   the Minimal Corresponding Source and Corresponding Application \
   Code. If you use option 4d1, you must provide the Installation \
   Information in the manner specified by section 6 of the GNU GPL \
   for conveying Corresponding Source.)\n</p>\
\n\
<p>  <b>5.</b> Combined Libraries.\n</p>\
\n\
<p>  You may place library facilities that are a work based on the \
Library side by side in a single library together with other library \
facilities that are not Applications and are not covered by this \
License, and convey such a combined library under terms of your \
choice, if you do both of the following:\n</p>\
\n\
<p>   <b>a)</b> Accompany the combined library with a copy of the same work based \
   on the Library, uncombined with any other library facilities, \
   conveyed under the terms of this License.\n</p>\
\n\
<p>   <b>b)</b> Give prominent notice with the combined library that part of it \
   is a work based on the Library, and explaining where to find the \
   accompanying uncombined form of the same work.\n</p>\
\n\
<p>  <b>6.</b> Revised Versions of the GNU Lesser General Public License.\n</p>\
\n\
<p>  The Free Software Foundation may publish revised and/or new versions \
of the GNU Lesser General Public License from time to time. Such new \
versions will be similar in spirit to the present version, but may \
differ in detail to address new problems or concerns.\n</p>\
\n\
<p>  Each version is given a distinguishing version number. If the \
Library as you received it specifies that a certain numbered version \
of the GNU Lesser General Public License \"or any later version\" \
applies to it, you have the option of following the terms and \
conditions either of that published version or of any later version \
published by the Free Software Foundation. If the Library as you \
received it does not specify a version number of the GNU Lesser \
General Public License, you may choose any version of the GNU Lesser \
General Public License ever published by the Free Software Foundation.\n</p>\
\n\
<p>  If the Library as you received it specifies that a proxy can decide \
whether future versions of the GNU Lesser General Public License shall \
apply, that proxy's public statement of acceptance of any version is \
permanent authorization for you to choose that version for the \
Library.\n</p>"
            }

            Rectangle {
                width: parent.width
                height: 2
                border.color: "lightGray"
                border.width: 1
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                text: "<p></p><p>\nCopyright 2011 The <b>LibYuv</b> Project Authors. All rights reserved.\n</p>\
\n\
<p>Redistribution and use in source and binary forms, with or without \
modification, are permitted provided that the following conditions are \
met:\n</p>\
\n\
<p>  <ul><li> Redistributions of source code must retain the above copyright \
    notice, this list of conditions and the following disclaimer.\n</li>\
\n\
  <li> Redistributions in binary form must reproduce the above copyright \
    notice, this list of conditions and the following disclaimer in \
    the documentation and/or other materials provided with the \
    distribution.\n</li>\
\n\
  <li> Neither the name of Google nor the names of its contributors may \
    be used to endorse or promote products derived from this software \
    without specific prior written permission.\n</li></ul></p>\
\n\
<p>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \
\"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT \
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR \
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT \
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, \
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT \
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, \
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY \
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT \
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE \
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n</p>"
            }

            Rectangle {
                width: parent.width
                height: 2
                border.color: "lightGray"
                border.width: 1
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                onLinkActivated: (link) => Qt.openUrlExternally(link)
                text: "<p>\n</p><p><b>QZXing License</b>\n</p>\
\n\
<p>   Apache License\n</p>\
<p>   Version 2.0, January 2004\n</p>\
<p>   <a href=\"http://www.apache.org/licenses/\">http://www.apache.org/licenses/</a>\n</p>\
\n\
<p>   <b>TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION</b>\n</p>\
\n\
<p>   <b>1.</b> Definitions.\n</p>\
\n\
<p>      \"License\" shall mean the terms and conditions for use, reproduction, \
      and distribution as defined by Sections 1 through 9 of this document.\n</p>\
\n\
<p>      \"Licensor\" shall mean the copyright owner or entity authorized by \
      the copyright owner that is granting the License.\n</p>\
\n\
<p>      \"Legal Entity\" shall mean the union of the acting entity and all \
      other entities that control, are controlled by, or are under common \
      control with that entity. For the purposes of this definition, \
      \"control\" means (i) the power, direct or indirect, to cause the \
      direction or management of such entity, whether by contract or \
      otherwise, or (ii) ownership of fifty percent (50%) or more of the \
      outstanding shares, or (iii) beneficial ownership of such entity.\n</p>\
\n\
<p>      \"You\" (or \"Your\") shall mean an individual or Legal Entity \
      exercising permissions granted by this License.\n</p>\
\n\
<p>      \"Source\" form shall mean the preferred form for making modifications, \
      including but not limited to software source code, documentation \
      source, and configuration files.\n</p>\
\n\
<p>      \"Object\" form shall mean any form resulting from mechanical \
      transformation or translation of a Source form, including but \
      not limited to compiled object code, generated documentation, \
      and conversions to other media types.\n</p>\
\n\
<p>      \"Work\" shall mean the work of authorship, whether in Source or \
      Object form, made available under the License, as indicated by a \
      copyright notice that is included in or attached to the work \
      (an example is provided in the Appendix below).\n</p>\
\n\
<p>      \"Derivative Works\" shall mean any work, whether in Source or Object \
      form, that is based on (or derived from) the Work and for which the \
      editorial revisions, annotations, elaborations, or other modifications \
      represent, as a whole, an original work of authorship. For the purposes \
      of this License, Derivative Works shall not include works that remain \
      separable from, or merely link (or bind by name) to the interfaces of, \
      the Work and Derivative Works thereof.\n</p>\
\n\
<p>      \"Contribution\" shall mean any work of authorship, including \
      the original version of the Work and any modifications or additions \
      to that Work or Derivative Works thereof, that is intentionally \
      submitted to Licensor for inclusion in the Work by the copyright owner \
      or by an individual or Legal Entity authorized to submit on behalf of \
      the copyright owner. For the purposes of this definition, \"submitted\" \
      means any form of electronic, verbal, or written communication sent \
      to the Licensor or its representatives, including but not limited to \
      communication on electronic mailing lists, source code control systems, \
      and issue tracking systems that are managed by, or on behalf of, the \
      Licensor for the purpose of discussing and improving the Work, but \
      excluding communication that is conspicuously marked or otherwise \
      designated in writing by the copyright owner as \"Not a Contribution.\"\n</p>\
\n\
<p>      \"Contributor\" shall mean Licensor and any individual or Legal Entity \
      on behalf of whom a Contribution has been received by Licensor and \
      subsequently incorporated within the Work.\n</p>\
\n\
<p>   <b>2.</b> Grant of Copyright License. Subject to the terms and conditions of \
      this License, each Contributor hereby grants to You a perpetual, \
      worldwide, non-exclusive, no-charge, royalty-free, irrevocable \
      copyright license to reproduce, prepare Derivative Works of, \
      publicly display, publicly perform, sublicense, and distribute the \
      Work and such Derivative Works in Source or Object form.\n </p>\
\n\
<p>   <b>3.</b> Grant of Patent License. Subject to the terms and conditions of \
      this License, each Contributor hereby grants to You a perpetual, \
      worldwide, non-exclusive, no-charge, royalty-free, irrevocable \
      (except as stated in this section) patent license to make, have made, \
      use, offer to sell, sell, import, and otherwise transfer the Work, \
      where such license applies only to those patent claims licensable \
      by such Contributor that are necessarily infringed by their \
      Contribution(s) alone or by combination of their Contribution(s) \
      with the Work to which such Contribution(s) was submitted. If You \
      institute patent litigation against any entity (including a \
      cross-claim or counterclaim in a lawsuit) alleging that the Work \
      or a Contribution incorporated within the Work constitutes direct \
      or contributory patent infringement, then any patent licenses \
      granted to You under this License for that Work shall terminate \
      as of the date such litigation is filed.\n</p>\
\n\
<p>   <b>4.</b> Redistribution. You may reproduce and distribute copies of the \
      Work or Derivative Works thereof in any medium, with or without \
      modifications, and in Source or Object form, provided that You \
      meet the following conditions:\n</p>\
\n\
<p>      <b>(a)</b> You must give any other recipients of the Work or \
          Derivative Works a copy of this License; and\n</p>\
\n\
<p>      <b>(b)</b> You must cause any modified files to carry prominent notices \
          stating that You changed the files; and\n</p>\
\n\
<p>      <b>(c)</b> You must retain, in the Source form of any Derivative Works \
          that You distribute, all copyright, patent, trademark, and \
          attribution notices from the Source form of the Work, \
          excluding those notices that do not pertain to any part of \
          the Derivative Works; and\n</p>\
\n\
<p>      <b>(d)</b> If the Work includes a \"NOTICE\" text file as part of its \
          distribution, then any Derivative Works that You distribute must \
          include a readable copy of the attribution notices contained \
          within such NOTICE file, excluding those notices that do not \
          pertain to any part of the Derivative Works, in at least one \
          of the following places: within a NOTICE text file distributed \
          as part of the Derivative Works; within the Source form or \
          documentation, if provided along with the Derivative Works; or, \
          within a display generated by the Derivative Works, if and \
          wherever such third-party notices normally appear. The contents \
          of the NOTICE file are for informational purposes only and \
          do not modify the License. You may add Your own attribution \
          notices within Derivative Works that You distribute, alongside \
          or as an addendum to the NOTICE text from the Work, provided \
          that such additional attribution notices cannot be construed \
          as modifying the License.\n</p>\
\n\
<p>      You may add Your own copyright statement to Your modifications and \
      may provide additional or different license terms and conditions \
      for use, reproduction, or distribution of Your modifications, or \
      for any such Derivative Works as a whole, provided Your use, \
      reproduction, and distribution of the Work otherwise complies with \
      the conditions stated in this License.\n</p>\
\n\
<p>   <b>5.</b> Submission of Contributions. Unless You explicitly state otherwise, \
      any Contribution intentionally submitted for inclusion in the Work \
      by You to the Licensor shall be under the terms and conditions of \
      this License, without any additional terms or conditions. \
      Notwithstanding the above, nothing herein shall supersede or modify \
      the terms of any separate license agreement you may have executed \
      with Licensor regarding such Contributions.\n</p>\
\n\
<p>   <b>6.</b> Trademarks. This License does not grant permission to use the trade \
      names, trademarks, service marks, or product names of the Licensor, \
      except as required for reasonable and customary use in describing the \
      origin of the Work and reproducing the content of the NOTICE file.\n</p>\
\n\
<p>   <b>7.</b> Disclaimer of Warranty. Unless required by applicable law or \
      agreed to in writing, Licensor provides the Work (and each \
      Contributor provides its Contributions) on an \"AS IS\" BASIS, \
      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or \
      implied, including, without limitation, any warranties or conditions \
      of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A \
      PARTICULAR PURPOSE. You are solely responsible for determining the \
      appropriateness of using or redistributing the Work and assume any \
      risks associated with Your exercise of permissions under this License.\n</p>\
\n\
<p>   <b>8.</b> Limitation of Liability. In no event and under no legal theory, \
      whether in tort (including negligence), contract, or otherwise, \
      unless required by applicable law (such as deliberate and grossly \
      negligent acts) or agreed to in writing, shall any Contributor be \
      liable to You for damages, including any direct, indirect, special, \
      incidental, or consequential damages of any character arising as a \
      result of this License or out of the use or inability to use the \
      Work (including but not limited to damages for loss of goodwill, \
      work stoppage, computer failure or malfunction, or any and all \
      other commercial damages or losses), even if such Contributor \
      has been advised of the possibility of such damages.\n</p>\
\n\
<p>   <b>9.</b> Accepting Warranty or Additional Liability. While redistributing \
      the Work or Derivative Works thereof, You may choose to offer, \
      and charge a fee for, acceptance of support, warranty, indemnity, \
      or other liability obligations and/or rights consistent with this \
      License. However, in accepting such obligations, You may act only \
      on Your own behalf and on Your sole responsibility, not on behalf \
      of any other Contributor, and only if You agree to indemnify, \
      defend, and hold each Contributor harmless for any liability \
      incurred by, or claims asserted against, such Contributor by reason \
      of your accepting any such warranty or additional liability.\n</p>\
\n\
<p>   END OF TERMS AND CONDITIONS\n</p>\
\n\
<p>   APPENDIX: How to apply the Apache License to your work.\n</p>\
\n\
<p>      To apply the Apache License to your work, attach the following \
      boilerplate notice, with the fields enclosed by brackets \"{}\" \
      replaced with your own identifying information. (Don't include \
      the brackets!)  The text should be enclosed in the appropriate \
      comment syntax for the file format. We also recommend that a \
      file or class name and description of purpose be included on the \
      same \"printed page\" as the copyright notice for easier \
      identification within third-party archives.\n</p>\
\n\
<p>   Copyright {yyyy} {name of copyright owner}\n</p>\
\n\
<p>   Licensed under the Apache License, Version 2.0 (the \"License\"); \
   you may not use this file except in compliance with the License. \
   You may obtain a copy of the License at\n</p>\
\n\
<p>       <a href=\"http://www.apache.org/licenses/LICENSE-2.0\">http://www.apache.org/licenses/LICENSE-2.0</a></p>\n\
\n\
<p>   Unless required by applicable law or agreed to in writing, software \
   distributed under the License is distributed on an \"AS IS\" BASIS, \
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. \
   See the License for the specific language governing permissions and \
   limitations under the License.\n</p>"
        }

            Rectangle {
                width: parent.width
                height: 2
                border.color: "lightGray"
                border.width: 1
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                onLinkActivated: (link) => Qt.openUrlExternally(link)
                text: "<p>\n</p><p><b>The Oxygen Icon Theme</b>\n</p>\
    \n\
<p>        Copyright (C) 2007 Nuno Pinheiro &lt;nuno@oxygen-icons.org&gt;\n</p>\
<p>        Copyright (C) 2007 David Vignoni &lt;david@icon-king.com&gt;\n</p>\
<p>        Copyright (C) 2007 David Miller &lt;miller@oxygen-icons.org&gt;\n</p>\
<p>        Copyright (C) 2007 Johann Ollivier Lapeyre &lt;johann@oxygen-icons.org&gt;\n</p>\
<p>        Copyright (C) 2007 Kenneth Wimer &lt;kwwii@bootsplash.org&gt;\n</p>\
<p>        Copyright (C) 2007 Riccardo Iaconelli &lt;riccardo@oxygen-icons.org&gt;\n</p>\
    \n\
    \n\
<p>    and others\n<\p>\
    \n\
<p>        This library is free software; you can redistribute it and/or \
        modify it under the terms of the GNU Lesser General Public \
        License as published by the Free Software Foundation; either \
        version 3 of the License, or (at your option) any later version.\n</p>\
    \n\
<p>        This library is distributed in the hope that it will be useful, \
        but WITHOUT ANY WARRANTY; without even the implied warranty of \
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU \
        Lesser General Public License for more details.\n</p>\
    \n\
<p>        You should have received a copy of the GNU Lesser General Public \
        License along with this library. If not, see \
<a href=\"http://www.gnu.org/licenses/\">&lt;http://www.gnu.org/licenses/&gt;</a>.\n</p>\
    \n\
<p>    Clarification:\n</p>\
    \n\
<p>      The GNU Lesser General Public License or LGPL is written for \
      software libraries in the first place. We expressly want the LGPL to \
      be valid for this artwork library too.\n</p>\
    \n\
<p>      KDE Oxygen theme icons is a special kind of software library, it is an \
      artwork library, it's elements can be used in a Graphical User Interface, or \
      GUI.\n</p>\
    \n\
<p>      Source code, for this library means:\n<\p>\
<ul>       <li>where they exist, SVG;\n</li>\
       <li>otherwise, if applicable, the multi-layered formats xcf or psd, or \
      otherwise png.\n</li></ul>\
    \n\
<p>      The LGPL in some sections obliges you to make the files carry \
      notices. With images this is in some cases impossible or hardly useful.\n</p>\
    \n\
<p>      With this library a notice is placed at a prominent place in the directory \
      containing the elements. You may follow this practice.\n</p>\
    \n\
<p>      The exception in section 5 of the GNU Lesser General Public License covers \
      the use of elements of this art library in a GUI.\n</p>\
    \n\
<p>      kde-artists [at] kde.org\n</p>\
    \n\
<p>               GNU LESSER GENERAL PUBLIC LICENSE\n</p>\
<p>               Version 3, 29 June 2007\n</p>\
    \n\
<p>     Copyright (C) 2007 Free Software Foundation, Inc. <a href=\"http://fsf.org/\">&lt;http://fsf.org/&gt;</a> \
     Everyone is permitted to copy and distribute verbatim copies \
     of this license document, but changing it is not allowed.\n</p>\
    \n\
    \n\
<p>      This version of the GNU Lesser General Public License incorporates \
    the terms and conditions of version 3 of the GNU General Public \
    License, supplemented by the additional permissions listed below.\n</p>\
    \n\
<p>      <b>0.</b> Additional Definitions. \n</p>\
    \n\
<p>      As used herein, \"this License\" refers to version 3 of the GNU Lesser \
    General Public License, and the \"GNU GPL\" refers to version 3 of the GNU \
    General Public License.\n</p>\
    \n\
<p>      \"The Library\" refers to a covered work governed by this License, \
    other than an Application or a Combined Work as defined below.\n</p>\
    \n\
<p>      An \"Application\" is any work that makes use of an interface provided \
    by the Library, but which is not otherwise based on the Library. \
    Defining a subclass of a class defined by the Library is deemed a mode \
    of using an interface provided by the Library.\n</p>\
    \n\
<p>      A \"Combined Work\" is a work produced by combining or linking an \
    Application with the Library.  The particular version of the Library \
    with which the Combined Work was made is also called the \"Linked \
    Version\".\n</p>\
    \n\
<p>      The \"Minimal Corresponding Source\" for a Combined Work means the \
    Corresponding Source for the Combined Work, excluding any source code \
    for portions of the Combined Work that, considered in isolation, are \
    based on the Application, and not on the Linked Version.\n</p>\
    \n\
<p>      The \"Corresponding Application Code\" for a Combined Work means the \
    object code and/or source code for the Application, including any data \
    and utility programs needed for reproducing the Combined Work from the \
    Application, but excluding the System Libraries of the Combined Work.\n</p>\
    \n\
<p>      <b>1.</b> Exception to Section 3 of the GNU GPL.\n</p>\
    \n\
<p>      You may convey a covered work under sections 3 and 4 of this License \
    without being bound by section 3 of the GNU GPL.\n</p>\
    \n\
<p>      <b>2.</b> Conveying Modified Versions.\n</p>\
    \n\
<p>      If you modify a copy of the Library, and, in your modifications, a \
    facility refers to a function or data to be supplied by an Application \
    that uses the facility (other than as an argument passed when the \
    facility is invoked), then you may convey a copy of the modified \
    version:\n</p>\
    \n\
<p>       <b>a)</b> under this License, provided that you make a good faith effort to \
       ensure that, in the event an Application does not supply the \
       function or data, the facility still operates, and performs \
       whatever part of its purpose remains meaningful, or\n</p>\
    \n\
<p>       <b>b)</b> under the GNU GPL, with none of the additional permissions of \
       this License applicable to that copy.\n</p>\
    \n\
<p>      <b>3.</b> Object Code Incorporating Material from Library Header Files.\n</p>\
    \n\
<p>      The object code form of an Application may incorporate material from \
    a header file that is part of the Library.  You may convey such object \
    code under terms of your choice, provided that, if the incorporated \
    material is not limited to numerical parameters, data structure \
    layouts and accessors, or small macros, inline functions and templates \
    (ten or fewer lines in length), you do both of the following:\n</p>\
    \n\
<p>       <b>a)</b> Give prominent notice with each copy of the object code that the \
       Library is used in it and that the Library and its use are \
       covered by this License.\n</p>\
    \n\
<p>       <b>b)</b> Accompany the object code with a copy of the GNU GPL and this license \
       document.\n</p>\
    \n\
<p>      <b>4.</b> Combined Works.\n</p>\
    \n\
<p>      You may convey a Combined Work under terms of your choice that, \
    taken together, effectively do not restrict modification of the \
    portions of the Library contained in the Combined Work and reverse \
    engineering for debugging such modifications, if you also do each of \
    the following:\n</p>\
    \n\
<p>       <b>a)</b> Give prominent notice with each copy of the Combined Work that \
       the Library is used in it and that the Library and its use are \
       covered by this License.\n</p>\
    \n\
<p>       <b>b)</b> Accompany the Combined Work with a copy of the GNU GPL and this license \
       document.\n</p>\
    \n\
<p>       <b>c)</b> For a Combined Work that displays copyright notices during \
       execution, include the copyright notice for the Library among \
       these notices, as well as a reference directing the user to the \
       copies of the GNU GPL and this license document.\n</p>\
    \n\
<p>       <b>d)</b> Do one of the following:\n</p>\
    \n\
<p>           <b>0)</b> Convey the Minimal Corresponding Source under the terms of this \
           License, and the Corresponding Application Code in a form \
           suitable for, and under terms that permit, the user to \
           recombine or relink the Application with a modified version of \
           the Linked Version to produce a modified Combined Work, in the \
           manner specified by section 6 of the GNU GPL for conveying \
           Corresponding Source.\n</p>\
    \n\
<p>           <b>1)</b> Use a suitable shared library mechanism for linking with the \
           Library.  A suitable mechanism is one that (a) uses at run time \
           a copy of the Library already present on the user's computer \
           system, and (b) will operate properly with a modified version \
           of the Library that is interface-compatible with the Linked \
           Version. \n</p>\
    \n\
<p>       <b>e)</b> Provide Installation Information, but only if you would otherwise \
       be required to provide such information under section 6 of the \
       GNU GPL, and only to the extent that such information is \
       necessary to install and execute a modified version of the \
       Combined Work produced by recombining or relinking the \
       Application with a modified version of the Linked Version. (If \
       you use option 4d0, the Installation Information must accompany \
       the Minimal Corresponding Source and Corresponding Application \
       Code. If you use option 4d1, you must provide the Installation \
       Information in the manner specified by section 6 of the GNU GPL \
       for conveying Corresponding Source.)\n</p>\
    \n\
<p>      <b>5.</b> Combined Libraries.\n</p>\
    \n\
<p>      You may place library facilities that are a work based on the \
    Library side by side in a single library together with other library \
    facilities that are not Applications and are not covered by this \
    License, and convey such a combined library under terms of your \
    choice, if you do both of the following:\n</p>\
    \n\
<p>       <b>a)</b> Accompany the combined library with a copy of the same work based \
       on the Library, uncombined with any other library facilities, \
       conveyed under the terms of this License.\n</p>\
    \n\
<p>       <b>b)</b> Give prominent notice with the combined library that part of it \
       is a work based on the Library, and explaining where to find the \
       accompanying uncombined form of the same work.\n</p>\
    \n\
<p>      <b>6.</b> Revised Versions of the GNU Lesser General Public License.\n</p>\
    \n\
<p>      The Free Software Foundation may publish revised and/or new versions \
    of the GNU Lesser General Public License from time to time. Such new \
    versions will be similar in spirit to the present version, but may \
    differ in detail to address new problems or concerns.\n</p>\
    \n\
<p>      Each version is given a distinguishing version number. If the \
    Library as you received it specifies that a certain numbered version \
    of the GNU Lesser General Public License \"or any later version\" \
    applies to it, you have the option of following the terms and \
    conditions either of that published version or of any later version \
    published by the Free Software Foundation. If the Library as you \
    received it does not specify a version number of the GNU Lesser \
    General Public License, you may choose any version of the GNU Lesser \
    General Public License ever published by the Free Software Foundation.\n</p>\
    \n\
<p>      If the Library as you received it specifies that a proxy can decide \
    whether future versions of the GNU Lesser General Public License shall \
    apply, that proxy's public statement of acceptance of any version is \
    permanent authorization for you to choose that version for the \
    Library.\n</p>"
            }

            Rectangle {
                width: parent.width
                height: 2
                border.color: "lightGray"
                border.width: 1
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                text: "<p></p><p><b>easy-encryption License</b>\n\n</p><p></p>\
<p>MIT License\n</p>\
\n\
<p>Copyright (c) 2018 Philippe Rémy\n</p>\
\n\
<p>Permission is hereby granted, free of charge, to any person obtaining a copy \
of this software and associated documentation files (the \"Software\"), to deal \
in the Software without restriction, including without limitation the rights \
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell \
copies of the Software, and to permit persons to whom the Software is \
furnished to do so, subject to the following conditions:\n</p>\
\n\
<p>The above copyright notice and this permission notice shall be included in all \
copies or substantial portions of the Software.\n</p>\
\n\
<p>THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR \
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE \
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, \
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE \
SOFTWARE.</p>"
            }
        }
    }
 }
