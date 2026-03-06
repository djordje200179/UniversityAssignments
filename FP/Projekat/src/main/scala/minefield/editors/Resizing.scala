package com.djordjemilanovic.minesweeper
package minefield.editors

import minefield.Minefield
import minefield.Minefield.Cell.*

object Resizing {
  def appendedRow(minefield: Minefield) = Minefield(minefield.mines.appended(Vector.fill(minefield.dimensions.width)(Empty)))
  def prependedRow(minefield: Minefield) = Minefield(minefield.mines.prepended(Vector.fill(minefield.dimensions.width)(Empty)))
  def appendedColumn(minefield: Minefield): Minefield = Minefield(minefield.mines.map(_.appended(Empty)))
  def prependedColumn(minefield: Minefield) = Minefield(minefield.mines.map(_.prepended(Empty)))

  def removedFirstRow(minefield: Minefield) = Minefield(minefield.mines.tail)
  def removedLastRow(minefield: Minefield) = Minefield(minefield.mines.init)
  def removedFirstColumn(minefield: Minefield) = Minefield(minefield.mines.map(_.tail))
  def removedLastColumn(minefield: Minefield) = Minefield(minefield.mines.map(_.init))
}
