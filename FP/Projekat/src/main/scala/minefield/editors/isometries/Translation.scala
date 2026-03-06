package com.djordjemilanovic.minesweeper
package minefield.editors.isometries

import minefield.Minefield
import minefield.editors.Isometry
import utils.Location

class Translation(dy: Int, dx: Int) extends Isometry {
  val offset: Location = Location(dy, dx)

  def apply(minefield: Minefield): Minefield = minefield

  def inverse: Isometry = Translation(-dy, -dx)
}
