import QtQuick
import Quickshell
import Quickshell.Io
import qs.Commons
import qs.Ui

// The Sofle layout diagram panel: every layer of the keymap, rendered from
// ~/.local/share/sofle/layers.json (generated from keymap.c by
// tools/gen-layout-json.py in ~/Projects/sofle). The section matching the live
// active layer — the same number the bar label reads — is highlighted
// while the panel is open.
Panel {
  id: root
  moduleName: "alvarosaavedra.sofle-layer"
  ipcTarget: "sofle-layer"
  manageIpc: false

  property var anchorItem: null
  readonly property string layersPath: (Quickshell.env("HOME") || "") + "/.local/share/sofle/layers.json"

  // The bar tracks the widget mounted in its slot — BarWidget.qml — not this
  // nested panel, so everything the bar identifies a panel by must be that
  // widget.
  property var hostWidget: null
  readonly property var barIdentity: hostWidget || root

  // Live layer off the host widget; binding (not injection) so the section
  // highlight follows layer changes while the panel is open.
  readonly property int activeLayer: hostWidget && hostWidget.activeLayerIndex !== undefined ? hostWidget.activeLayerIndex : -1

  property var layoutData: null
  readonly property var visibleLayers: {
    if (!layoutData || !layoutData.layers) return []
    var out = []
    for (var i = 0; i < layoutData.layers.length; i++) {
      var l = layoutData.layers[i]
      var has = false
      for (var r = 0; r < l.rows.length && !has; r++) {
        for (var k = 0; k < l.rows[r].length; k++) {
          var c = l.rows[r][k].c
          if (c !== "trans" && c !== "blank" && c !== "gap") { has = true; break }
        }
      }
      if (has) out.push(l)
    }
    return out
  }

  readonly property int keyUnit: Style.space(32)
  readonly property int keyGap: Math.max(3, Style.space(2))
  readonly property real diagramWidth: {
    var widest = 0
    var layers = visibleLayers
    for (var i = 0; i < layers.length; i++) {
      var rows = layers[i].rows
      for (var r = 0; r < rows.length; r++) {
        var w = 0
        for (var k = 0; k < rows[r].length; k++)
          w += keyUnit * (rows[r][k].w || 1) + keyGap
        widest = Math.max(widest, w - keyGap)
      }
    }
    return widest
  }

  function alpha(c, a) {
    return Qt.rgba(c.r, c.g, c.b, a)
  }

  FileView {
    path: root.layersPath
    watchChanges: true
    printErrors: false
    onFileChanged: reload()
    onLoaded: {
      try {
        root.layoutData = JSON.parse(text())
      } catch (e) {
        root.layoutData = null
      }
    }
    onLoadFailed: root.layoutData = null
  }

  // ---- Lifecycle. Shape contract shared with the clock panel:
  //      open/close/opened plus popout-switch forwarding.
  readonly property bool opened: controller.open

  function open() {
    controller.show()
    Qt.callLater(function() {
      console.log("sofle-layer cardOrigin=" + panel.cardOrigin + " anchor=" + panel.anchorScreenPos
        + " anchorW=" + panel.anchorW + " contentW=" + panel.contentWidth
        + " screenW=" + panel.screenW + " centerOnBar=" + panel.centerOnBar)
    })
  }

  function close() {
    controller.hide()
  }

  function toggle() {
    if (root.opened) root.close()
    else root.open()
  }

  function switchPanel(direction) {
    if (root.bar && typeof root.bar.switchPanelFrom === "function")
      return root.bar.switchPanelFrom(root.barIdentity, direction)
    return false
  }

  // Forwarded so this widget can stand in for the panel as the bar's popout
  // identity.
  property bool popoutSwitchClosing: false

  function closeForPopoutSwitch() {
    popoutSwitchClosing = true
    root.close()
    Qt.callLater(function() { popoutSwitchClosing = false })
  }

  KeyboardPanel {
    id: panel
    anchorItem: root.anchorItem
    owner: root.barIdentity
    bar: root.bar
    open: root.opened
    // Anchored under the widget (clamped to the screen) rather than centered
    // on the bar: the widget can live in any bar section.
    contentWidth: panel.fittedContentWidth(Math.round(root.diagramWidth))
    contentHeight: panel.fittedContentHeight(contentColumn.implicitHeight)

    Flickable {
      anchors.fill: parent
      contentWidth: contentColumn.width
      contentHeight: contentColumn.implicitHeight
      clip: true
      boundsBehavior: Flickable.StopAtBounds

      Column {
        id: contentColumn
        width: Math.max(root.diagramWidth, childrenRect.width)
        spacing: Style.space(4)

        Text {
          text: "Sofle keymap"
          color: Color.popups.text
          font.family: root.bar ? root.bar.fontFamily : Style.font.family
          font.pixelSize: Style.font.subtitle
          font.bold: true
        }

        Repeater {
          model: root.visibleLayers

          LayerSection {
            required property var modelData
            layerData: modelData
          }
        }

        Text {
          text: "generated from ~/Projects/sofle keymap.c"
          color: root.alpha(Color.popups.text, 0.45)
          font.family: root.bar ? root.bar.fontFamily : Style.font.family
          font.pixelSize: Style.font.caption
        }
      }
    }
  }

  component LayerSection: Column {
    id: section
    property var layerData: null
    readonly property bool active: layerData && layerData.index === root.activeLayer
    spacing: Math.max(2, Style.space(1))

    Row {
      spacing: Style.space(2)

      Rectangle {
        width: Style.space(2)
        height: headerLabel.implicitHeight
        radius: width / 2
        color: section.active ? Color.urgent : root.alpha(Color.popups.text, 0.25)
        visible: true
      }

      Text {
        id: headerLabel
        text: section.layerData ? section.layerData.name : ""
        color: section.active ? Color.popups.text : root.alpha(Color.popups.text, 0.7)
        font.family: root.bar ? root.bar.fontFamily : Style.font.family
        font.pixelSize: Style.font.bodySmall
        font.bold: section.active
        font.capitalization: Font.AllUppercase
      }

      Text {
        text: "active"
        visible: section.active
        color: Color.urgent
        font.family: root.bar ? root.bar.fontFamily : Style.font.family
        font.pixelSize: Style.font.caption
        anchors.baseline: headerLabel.baseline
      }
    }

    Column {
      spacing: root.keyGap

      Repeater {
        model: section.layerData ? section.layerData.rows : []

        Row {
          id: keyRow
          required property var modelData
          spacing: root.keyGap

          Repeater {
            model: keyRow.modelData

            KeyCell {
              required property var modelData
              keyData: modelData
            }
          }
        }
      }
    }
  }

  component KeyCell: Rectangle {
    id: cell
    property var keyData: null
    readonly property string cls: keyData ? keyData.c : "gap"
    readonly property bool isGap: cls === "gap"
    readonly property bool isBlank: cls === "blank"
    readonly property bool isTrans: cls === "trans"
    readonly property bool isMo: cls === "mo"
    readonly property bool isEnc: cls === "enc"

    width: isGap ? root.keyUnit * 0.6 : root.keyUnit * (keyData.w || 1)
    height: root.keyUnit
    radius: isEnc ? height / 2 : Math.max(2, Style.space(1))
    color: cls === "mod"
      ? root.alpha(Color.popups.text, 0.07)
      : "transparent"
    border.width: isGap || isBlank ? 1 : 1
    border.color: {
      if (isGap) return "transparent"
      if (isBlank) return root.alpha(Color.popups.text, 0.12)
      if (isMo) return Color.urgent
      if (isEnc) return root.alpha(Color.popups.text, 0.45)
      if (isTrans) return root.alpha(Color.popups.text, 0.18)
      return Color.popups.border
    }

    Text {
      anchors.centerIn: parent
      width: parent.width - 2
      text: cell.keyData ? (cell.keyData.t || "") : ""
      elide: Text.ElideRight
      horizontalAlignment: Text.AlignHCenter
      color: {
        if (cell.isMo) return Color.urgent
        if (cell.isTrans || cell.isBlank) return root.alpha(Color.popups.text, 0.4)
        if (cell.isEnc) return root.alpha(Color.popups.text, 0.6)
        return Color.popups.text
      }
      font.family: root.bar ? root.bar.fontFamily : Style.font.family
      font.pixelSize: Style.font.caption
      font.bold: cell.isMo
    }
  }
}
