package com.djordjemilanovic.minesweeper
package scenes

import components.NavigationButton
import dialogs.CustomMinefieldFileDialog
import dialogs.isometries.*
import minefield.Minefield
import minefield.Minefield.Cell.*
import minefield.editors.{CellEditing, Isometry, Resizing}
import utils.{Location, Sector}

import scalafx.Includes.jfxMouseEvent2sfx
import scalafx.beans.property.{BooleanProperty, ObjectProperty}
import scalafx.geometry.Pos
import scalafx.scene.Scene
import scalafx.scene.control.{Button, CheckBox, Label, ToggleButton}
import scalafx.scene.input.MouseButton.{Primary, Secondary}
import scalafx.scene.layout.{GridPane, HBox, VBox}
import scalafx.scene.paint.Color.LightBlue

import scala.collection.immutable.HashMap
import scala.util.{Success, Try}

class LevelCreatorScene(originalMinefield: Minefield) extends Scene {
  private val observableMinefield = new ObjectProperty[Minefield]
  private val buttonsGrid = new GridPane {
    hgap = 2
    vgap = 2
  }
  private val difficultyLabel = new Label(s"Difficulty: ${originalMinefield.difficulty}")

  private val minePlacingMode = new BooleanProperty

  private var upperLeftSelectionCell = Option.empty[Location]
  private var lowerRightSelectionCell = Option.empty[Location]

  private val isometries =
    HashMap.newBuilder[String, Class[? <: IsometryCreatorDialog]]
      .addOne("Rotate" -> classOf[RotationDialog])
      .addOne("Reflect" -> classOf[ReflectionDialog])
      .addOne("Translate" -> classOf[TranslationDialog])
      .addOne("Central symmetry" -> classOf[CentralSymmetryDialog])
      .result()

  private val expandingIsometry = new BooleanProperty
  private val transparentIsometry = new BooleanProperty

  private def redraw(minefield: Minefield): Unit = {
    buttonsGrid.children.clear()

    difficultyLabel.text = s"Difficulty: ${minefield.difficulty}"

    for (y <- 0 until minefield.dimensions.height; x <- 0 until minefield.dimensions.width) {
      val location = Location(y, x)
      val cell = minefield(location)
      val btn = new Button {
        text = cell.toString
        minWidth = 30
        minHeight = 30
        style = (upperLeftSelectionCell, lowerRightSelectionCell) match {
          case (Some(ul), Some(lr)) if location.y >= ul.y && location.y <= lr.y && location.x >= ul.x && location.x <= lr.x => "-fx-background-color: pink;"
          case (Some(ul), None) if location == ul => "-fx-background-color: yellow;"
          case _ => ""
        }
        onMouseClicked = e => {
          if (minePlacingMode.value) {
            (e.button match {
              case Primary => Some(Mine)
              case Secondary => Some(Empty)
              case _ => None
            }).foreach { nextCell => updateMinefield(CellEditing.updatedCell(nextCell, location)) }
          } else if (e.button == Primary) {
            if (upperLeftSelectionCell.isDefined && lowerRightSelectionCell.isDefined) {
              upperLeftSelectionCell = Some(location)
              lowerRightSelectionCell = None
            } else if (upperLeftSelectionCell.isEmpty)
              upperLeftSelectionCell = Some(location)
            else if (lowerRightSelectionCell.isEmpty)
              lowerRightSelectionCell = Some(location)

            redraw(minefield)
          }
        }
      }
      buttonsGrid.add(btn, x, y)
    }
  }

  private def createMinefieldResizeButtons(increaseAction: Minefield => Minefield, decreaseAction: Minefield => Minefield) = Seq(
    createMinefieldActionButton("+", increaseAction),
    createMinefieldActionButton("-", decreaseAction)
  )

  observableMinefield.onChange { (_, _, newMinefield) => redraw(newMinefield) }
  observableMinefield.value = originalMinefield

  private def createMinefieldActionButton(label: String, action: Minefield => Minefield) = new Button(label) {
    onAction = _ => updateMinefield(action)
    style = "-fx-font-size: 16pt; -fx-font-weight: bold;"
  }

  private def updateMinefield(mapper: Minefield => Minefield): Unit =
    observableMinefield.value = mapper(observableMinefield.value)

  fill = LightBlue
  content = new VBox(5) {
    alignment = Pos.Center
    children = Seq(
      new HBox(5) {
        children = Seq(
          NavigationButton.Back(),
          new Button("Save") {
            onAction = _ => {
              CustomMinefieldFileDialog.save()
                .fold(Success(())) { fileAccess =>
                  for {
                    file <- fileAccess
                    result <- Serialization.writeMinefield(observableMinefield.value)(file)
                  } yield result
                }.get
            }
          },
          new ToggleButton("Set mines") {
            selected <==> minePlacingMode
          },
          new Button("Clear sector") {
            disable <== minePlacingMode
            onAction = _ => updateMinefield(CellEditing.clearedSector(Sector(
              upperLeftSelectionCell.get,
              lowerRightSelectionCell.get
            )))
          },
        )
      },
      new HBox(5) {
        children = isometries.map { case (name, dialog) =>
          new Button(name) {
            disable <== minePlacingMode
            onAction = _ => {
              val sector = Sector(
                upperLeftSelectionCell.get,
                lowerRightSelectionCell.get
              )
              Try(
                dialog
                  .getDeclaredConstructor(classOf[Minefield])
                  .newInstance(observableMinefield.value.extracted(sector))
              ).getOrElse(
                dialog
                  .getDeclaredConstructor()
                  .newInstance()
              )
              .create
              .map(Isometry(expandingIsometry.value, !transparentIsometry.value))
              .map(_.apply(sector))
              .foreach(updateMinefield)

              upperLeftSelectionCell = None
              lowerRightSelectionCell = None
              redraw(observableMinefield.value)
            }
          }
        }
      },
      new HBox(5) {
        children = Seq(
          new CheckBox("Expanding isometry") {
            selected <==> expandingIsometry
          },
          new CheckBox("Transparent isometry") {
            selected <==> transparentIsometry
          }
        )
      },
      difficultyLabel,
      new HBox(5) {
        alignment = Pos.Center
        children = createMinefieldResizeButtons(Resizing.prependedRow, Resizing.removedFirstRow)
      },
      new HBox(5) {
        alignment = Pos.Center
        children = Seq(
          new VBox(5) {
            alignment = Pos.Center
            children = createMinefieldResizeButtons(Resizing.prependedColumn, Resizing.removedFirstColumn)
          },
          buttonsGrid,
          new VBox(5) {
            alignment = Pos.Center
            children = createMinefieldResizeButtons(Resizing.appendedColumn, Resizing.removedLastColumn)
          }
        )
      },
      new HBox(5) {
        alignment = Pos.Center
        children = createMinefieldResizeButtons(Resizing.appendedRow, Resizing.removedLastRow)
      }
    )
  }
}
