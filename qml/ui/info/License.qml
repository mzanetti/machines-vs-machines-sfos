import QtQuick 2.2
import "../../components"
import harbour.machines.vs.machines.sfos.Machines 1.0
import QtQuick.Layouts 1.1

ModalOverlay {
    id: root
    title: "Software license"
    showCloseButton: true

    Flickable {
        anchors.fill: parent
        anchors.margins: app.margins
        contentHeight: label.height
        clip: true

        GameLabel {
            id: label
            width: parent.width
            text: "This program is free software: you can redistribute it and/or modify " +
                "it under the terms of the GNU General Public License as published by " +
                "the Free Software Foundation, version 3 of the License.<br><br>" +
                "This program is distributed in the hope that it will be useful, " +
                "but WITHOUT ANY WARRANTY; without even the implied warranty of " +
                "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the " +
                "GNU General Public License for more details.<br><br>" +
                "You should have received a copy of the GNU General Public License " +
                "along with this program.  If not, see " +
                "<a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>."
            wrapMode: Text.WordWrap
        }
    }
}
