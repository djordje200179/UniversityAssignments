package com.djordjemilanovic.minesweeper
package components

import scalafx.scene.Scene
import scalafx.scene.control.Button

class NavigationButton(private val name: String, private val nextScene: Scene) extends Button(name) {
  onAction = _ => App.navigate(nextScene)
}

object NavigationButton {
  class Back extends NavigationButton("Back to menu", scenes.MenuScene) {}
}