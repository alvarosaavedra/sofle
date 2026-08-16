import QtQuick
import Quickshell
import Quickshell.Io
import qs.Commons
import qs.Ui

// Live Sofle layer indicator.
//
// The sofle-layerd user service mirrors the keyboard's active QMK layer
// (printed on the firmware's debug console by layer_state_set_user) into
// $XDG_RUNTIME_DIR/sofle-layer; this widget watches that file and shows
// "Sofle" on the base layer, "Lower"/"Raise"/... highlighted otherwise.
//
// Click opens the layout panel: a diagram of every layer, generated from
// keymap.c by tools/gen-layout-json.py into
// ~/.local/share/sofle/layers.json.
BarWidget {
  id: root
  moduleName: "alvarosaavedra.sofle-layer"

  readonly property string home: Quickshell.env("HOME") || "/home/radbug"
  readonly property string runtimeDir: Quickshell.env("XDG_RUNTIME_DIR") || "/run/user/1000"
  readonly property string statePath: runtimeDir + "/sofle-layer"
  readonly property string layersPath: home + "/.local/share/sofle/layers.json"

  // Not `layer`: Item already declares a FINAL `layer` property (visual
  // effect group), which this would collide with.
  property int activeLayerIndex: 0
  readonly property var layerNames: ["Qwerty", "Lower", "Raise", "Adjust"]
  readonly property string layerName: activeLayerIndex >= 0 && activeLayerIndex < layerNames.length
    ? layerNames[activeLayerIndex]
    : ("L" + activeLayerIndex)
  // Keyboard icon: nf-fa-keyboard (U+F11C), native in the bar's Nerd Font
  // monospace face. Base layer is icon-only; an active layer appends its
  // name (" Lower").
  readonly property string keyIcon: "\uF11C"
  readonly property string displayText: activeLayerIndex === 0 ? keyIcon : keyIcon + " " + layerName

  // ---- Layout panel. Shape contract for shell.summon/hide/toggle routing:
  //      Bar.findPanelWidget requires open/close/opened on the bar-widget
  //      root.
  readonly property bool opened: panelLoader.item ? panelLoader.item.opened === true : false

  function open() {
    if (panelLoader.item) panelLoader.item.open()
  }

  function close() {
    if (panelLoader.item) panelLoader.item.close()
  }

  function togglePanel() {
    if (panelLoader.item) panelLoader.item.toggle()
  }

  readonly property real openPanelIndicatorWidth: button.labelWidth

  function injectPanel() {
    var target = panelLoader.item
    if (!target) return
    if ("bar" in target) target.bar = root.bar
    if ("anchorItem" in target) target.anchorItem = button
    if ("hostWidget" in target) target.hostWidget = root
  }

  implicitWidth: button.implicitWidth
  implicitHeight: button.implicitHeight

  onBarChanged: injectPanel()
  onSettingsChanged: injectPanel()

  FileView {
    path: root.statePath
    watchChanges: true
    printErrors: false
    onFileChanged: reload()
    onLoaded: {
      var parsed = parseInt(text().trim(), 10)
      root.activeLayerIndex = isNaN(parsed) ? 0 : Math.max(0, Math.min(9, parsed))
    }
    onLoadFailed: root.activeLayerIndex = 0
  }

  Loader {
    id: panelLoader
    active: true
    source: Qt.resolvedUrl("Panel.qml")
    visible: false
    onLoaded: {
      root.injectPanel()
      Qt.callLater(root.injectPanel)
    }
  }

  IpcHandler {
    target: "sofle-layer"

    function open(): void { root.open() }
    function close(): void { root.close() }
    function show(): void { root.open() }
    function hide(): void { root.close() }
    function toggle(): void { root.togglePanel() }
  }

  WidgetButton {
    id: button
    anchors.fill: parent
    bar: root.bar
    text: root.displayText
    fontSize: root.activeLayerIndex === 0 ? Style.bar.iconFont : Style.font.bodySmall
    horizontalMargin: 6
    active: root.activeLayerIndex !== 0
    tooltipText: "Sofle — " + root.layerName + " (click for the layout)"
    onPressed: function(b) { root.togglePanel() }
  }
}
