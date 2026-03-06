package com.djordjemilanovic.minesweeper
package minefield.editors

import minefield.Minefield
import minefield.Minefield.Cell
import utils.{Location, Sector}

object CellEditing {
  def updatedCell(newCell: Cell, location: Location)(minefield: Minefield) =
    Minefield(minefield.mines.updated(location.y, minefield.mines(location.y).updated(location.x, newCell)))

  def clearedSector(sector: Sector)(minefield: Minefield): Minefield =
    minefield.combined(false, true)(sector.upperLeft)(Minefield.empty(sector.dimensions))
}
