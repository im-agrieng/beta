import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Rectangle {
  id: sensorInformationView

  property alias activeSensors: grid.count

  property int ceilsCount: 4
  property double cellHeight: 26
  property double cellPadding: 6
  property color backgroundColor: Theme.mainBackgroundColor
  property color alternateBackgroundColor: Theme.sensorBackgroundColor
  property color textColor: Theme.mainTextColor
  property real contentHeight: parent.width > 620 ? cellHeight * Math.ceil(grid.count / 3) : cellHeight * Math.ceil(grid.count / 2)

  width: parent.width
  anchors.margins: 20

  color: Theme.mainBackgroundColor

  GridView {
    id: grid
    Layout.fillWidth: true
    Layout.preferredHeight: childrenRect.height
    width: parent.width
    height: parent.height
    cellWidth: parent.width > 620 ? parent.width / 3 : parent.width / 2
    cellHeight: sensorInformationView.cellHeight
    flow: GridLayout.TopToBottom

    model: SensorListModel {
      project: qgisProject
      showConnectedOnly: true
    }

    delegate: Rectangle {
      width: grid.cellWidth
      height: grid.cellHeight
      color: index % 2 == 0 ? sensorInformationView.alternateBackgroundColor : sensorInformationView.backgroundColor

      RowLayout {
        anchors.margins: cellPadding
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
          Layout.fillWidth: false
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: SensorName
        }

        Text {
          Layout.fillWidth: true
          font: Theme.tipFont
          color: sensorInformationView.textColor
          text: SensorLastValue ? (SensorLastValue + '').trim() : qsTr("N/A")
          verticalAlignment: Text.AlignVCenter
          elide: Text.ElideRight
        }
      }
    }
  }
}
