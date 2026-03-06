package com.djordjemilanovic.minesweeper

import scenes.MenuScene

import scalafx.application.JFXApp3
import scalafx.scene.Scene
import scalafx.scene.control.Alert
import scalafx.scene.control.Alert.AlertType

object App extends JFXApp3 {
  Thread.setDefaultUncaughtExceptionHandler((_, e) =>
    new Alert(AlertType.Error) {
      title = "Unhandled error"
      headerText = e.getMessage
      contentText = e.getStackTrace.take(5).mkString(System.lineSeparator)
    }.showAndWait()
  )

  override def start(): Unit = {
    stage = new JFXApp3.PrimaryStage {
      title.value = "Minesweeper"
      width = 1000
      height = 800
      scene = MenuScene
    }
  }
  
  def navigate(scene: Scene): Unit = stage.scene = scene
}
