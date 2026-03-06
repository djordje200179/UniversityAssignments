package com.djordjemilanovic.minesweeper
package minefield.editors

import minefield.Minefield
import utils.{Location, Sector}

trait Isometry {
  val offset: Location

  def apply(minefield: Minefield): Minefield

  def inverse: Isometry
}

object Isometry {
  def apply(expand: Boolean, overwrite: Boolean)(isometry: Isometry)(sector: Sector)(minefield: Minefield): Minefield = {
    val transformedSector = isometry(minefield.extracted(sector))

    minefield.combined(expand, overwrite)(sector.upperLeft + isometry.offset)(transformedSector)
  }
}